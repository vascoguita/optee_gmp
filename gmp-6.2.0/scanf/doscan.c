/* __gmp_doscan -- formatted input internals.

   THE FUNCTIONS IN THIS FILE ARE FOR INTERNAL USE ONLY.  THEY'RE ALMOST
   CERTAIN TO BE SUBJECT TO INCOMPATIBLE CHANGES OR DISAPPEAR COMPLETELY IN
   FUTURE GNU MP RELEASES.

Copyright 2001-2003 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received copies of the GNU General Public License and the
GNU Lesser General Public License along with the GNU MP Library.  If not,
see https://www.gnu.org/licenses/.  */

#define _GNU_SOURCE    /* for DECIMAL_POINT in langinfo.h */

#include "config.h"	/* needed for the HAVE_, could also move gmp incls */

#include <stdarg.h>
#include <ctype.h>
#include <stddef.h>    /* for ptrdiff_t */
#include <stdio.h>
#include <stdlib.h>    /* for strtol */
#include <string.h>

#if HAVE_LANGINFO_H
#include <langinfo.h>  /* for nl_langinfo */
#endif

#if HAVE_LOCALE_H
#include <locale.h>    /* for localeconv */
#endif

#if HAVE_INTTYPES_H
# include <inttypes.h> /* for intmax_t */
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h> /* for quad_t */
#endif

#include "gmp-impl.h"


/* Change this to "#define TRACE(x) x" for some traces. */
#define TRACE(x)


/* General:

       It's necessary to parse up the format string to recognise the GMP
       extra types F, Q and Z.  Other types and conversions are passed
       across to the standard sscanf or fscanf via funs->scan, for ease of
       implementation.  This is essential in the case of something like glibc
       %p where the pointer format isn't actually documented.

       Because funs->scan doesn't get the whole input it can't put the right
       values in for %n, so that's handled in __gmp_doscan.  Neither sscanf
       nor fscanf directly indicate how many characters were read, so an
       extra %n is appended to each run for that.  For fscanf this merely
       supports our %n output, but for sscanf it lets funs->step move us
       along the input string.

       Whitespace and literal matches in the format string, including %%,
       are handled directly within __gmp_doscan.  This is reasonably
       efficient, and avoids some suspicious behaviour observed in various
       system libc's.  GLIBC 2.2.4 for instance returns 0 on

	   sscanf(" ", " x")
       or
	   sscanf(" ", " x%d",&n)

       whereas we think they should return EOF, since end-of-string is
       reached when a match of "x" is required.

       For standard % conversions, funs->scan is called once for each
       conversion.  If we had vfscanf and vsscanf and could rely on their
       fixed text matching behaviour then we could call them with multiple
       consecutive standard conversions.  But plain fscanf and sscanf work
       fine, and parsing one field at a time shouldn't be too much of a
       slowdown.

   gmpscan:

       gmpscan reads a gmp type.  It's only used from one place, but is a
       separate subroutine to avoid a big chunk of complicated code in the
       middle of __gmp_doscan.  Within gmpscan a couple of loopbacks make it
       possible to share code for parsing integers, rationals and floats.

       In gmpscan normally one char of lookahead is maintained, but when width
       is reached that stops, on the principle that an fgetc/ungetc of a char
       past where we're told to stop would be undesirable.  "chars" is how many
       characters have been read so far, including the current c.  When
       chars==width and another character is desired then a jump is done to the
       "convert" stage.  c is invalid and mustn't be unget'ed in this case;
       chars is set to width+1 to indicate that.

       gmpscan normally returns the number of characters read.  -1 means an
       invalid field, -2 means EOF reached before any matching characters
       were read.

       For hex floats, the mantissa part is passed to mpf_set_str, then the
       exponent is applied with mpf_mul_exp or mpf_div_2exp.  This is easier
       than teaching mpf_set_str about an exponent factor (ie. 2) differing
       from the mantissa radix point factor (ie. 16).  mpf_mul_exp and
       mpf_div_2exp will preserve the application requested precision, so
       nothing in that respect is lost by making this a two-step process.

   Matching and errors:

       C99 7.19.6.2 paras 9 and 10 say an input item is read as the longest
       string which is a match for the appropriate type, or a prefix of a
       match.  With that done, if it's only a prefix then the result is a
       matching failure, ie. invalid input.

       This rule seems fairly clear, but doesn't seem to be universally
       applied in system C libraries.  Even GLIBC doesn't seem to get it
       right, insofar as it seems to accept some apparently invalid forms.
       Eg. glibc 2.3.1 accepts "0x" for a "%i", where a reading of the
       standard would suggest a non-empty sequence of digits should be
       required after an "0x".

       A footnote to 7.19.6.2 para 17 notes how this input item reading can
       mean inputs acceptable to strtol are not acceptable to fscanf.  We
       think this confirms our reading of "0x" as invalid.

       Clearly gmp_sscanf could backtrack to a longest input which was a
       valid match for a given item, but this is not done, since C99 says
       sscanf is identical to fscanf, so we make gmp_sscanf identical to
       gmp_fscanf.

   Types:

       C99 says "ll" is for long long, and "L" is for long double floats.
       Unfortunately in GMP 4.1.1 we documented the two as equivalent.  This
       doesn't affect us directly, since both are passed through to plain
       scanf.  It seems wisest not to try to enforce the C99 rule.  This is
       consistent with what we said before, though whether it actually
       worked was always up to the C library.

   Alternatives:

       Consideration was given to using separate code for gmp_fscanf and
       gmp_sscanf.  The sscanf case could zip across a string doing literal
       matches or recognising digits in gmpscan, rather than making a
       function call fun->get per character.  The fscanf could use getc
       rather than fgetc too, which might help those systems where getc is a
       macro or otherwise inlined.  But none of this scanning and converting
       will be particularly fast, so the two are done together to keep it a
       little simpler for now.

       Various multibyte string issues are not addressed, for a start C99
       scanf says the format string is multibyte.  Since we pass %c, %s and
       %[ to the system scanf, they might do multibyte reads already, but
       it's another matter whether or not that can be used, since our digit
       and whitespace parsing is only unibyte.  The plan is to quietly
       ignore multibyte locales for now.  This is not as bad as it sounds,
       since GMP is presumably used mostly on numbers, which can be
       perfectly adequately treated in plain ASCII.

*/


struct gmp_doscan_params_t {
  int	base;
  int	ignore;
  char	type;
  int	width;
};


#define GET(c)			\
  do {				\
    ASSERT (chars <= width);	\
    chars++;			\
    if (chars > width)		\
      goto convert;		\
    (c) = (*funs->get) (data);	\
  } while (0)

/* store into "s", extending if necessary */
#define STORE(c)							\
  do {									\
    ASSERT (s_upto <= s_alloc);						\
    if (s_upto >= s_alloc)						\
      {									\
	size_t	s_alloc_new = s_alloc + S_ALLOC_STEP;			\
	s = __GMP_REALLOCATE_FUNC_TYPE (s, s_alloc, s_alloc_new, char); \
	s_alloc = s_alloc_new;						\
      }									\
    s[s_upto++] = c;							\
  } while (0)

#define S_ALLOC_STEP  512


/* Read and discard whitespace, if any.  Return number of chars skipped.
   Whitespace skipping never provokes the EOF return from __gmp_doscan, so
   it's not necessary to watch for EOF from funs->get, */
static int
skip_white (const struct gmp_doscan_funs_t *funs, void *data)
{
  int  c;
  int  ret = 0;

  do
    {
      c = (funs->get) (data);
      ret++;
    }
  while (isspace (c));

  (funs->unget) (c, data);
  ret--;

  TRACE (printf ("  skip white %d\n", ret));
  return ret;
}