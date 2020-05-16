/* mpf_out_str (stream, base, n_digits, op) -- Print N_DIGITS digits from
   the float OP to STREAM in base BASE.  Return the number of characters
   written, or 0 if an error occurred.

Copyright 1996, 1997, 2001, 2002, 2005, 2011 Free Software Foundation, Inc.

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

#include "config.h"

#include <stdio.h>
#include <string.h>

#if HAVE_LANGINFO_H
#include <langinfo.h>  /* for nl_langinfo */
#endif

#if HAVE_LOCALE_H
#include <locale.h>    /* for localeconv */
#endif

#include "gmp-impl.h"
#include "longlong.h"
