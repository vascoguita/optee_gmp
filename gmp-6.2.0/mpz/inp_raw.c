/* mpz_inp_raw -- read an mpz_t in raw format.

Copyright 2001, 2002, 2005, 2012, 2016 Free Software Foundation, Inc.

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

#include <stdio.h>
#include "gmp-impl.h"


/* NTOH_LIMB_FETCH fetches a limb which is in network byte order (ie. big
   endian) and produces a normal host byte order result. */

#if HAVE_LIMB_BIG_ENDIAN
#define NTOH_LIMB_FETCH(limb, src)  do { (limb) = *(src); } while (0)
#endif

#if HAVE_LIMB_LITTLE_ENDIAN
#define NTOH_LIMB_FETCH(limb, src)  BSWAP_LIMB_FETCH (limb, src)
#endif

#ifndef NTOH_LIMB_FETCH
#define NTOH_LIMB_FETCH(limb, src)                              \
  do {                                                          \
    const unsigned char  *__p = (const unsigned char *) (src);  \
    mp_limb_t  __limb;                                          \
    int        __i;                                             \
    __limb = 0;                                                 \
    for (__i = 0; __i < GMP_LIMB_BYTES; __i++)               \
      __limb = (__limb << 8) | __p[__i];                        \
    (limb) = __limb;                                            \
  } while (0)
#endif


/* Enhancement: The byte swap loop ought to be safe to vectorize on Cray
   etc, but someone who knows what they're doing needs to check it.  */