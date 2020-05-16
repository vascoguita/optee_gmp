/* mpz_out_raw -- write an mpz_t in raw format.

Copyright 2001, 2002 Free Software Foundation, Inc.

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
#include "longlong.h"


/* HTON_LIMB_STORE takes a normal host byte order limb and stores it as
   network byte order (ie. big endian). */

#if HAVE_LIMB_BIG_ENDIAN
#define HTON_LIMB_STORE(dst, limb)  do { *(dst) = (limb); } while (0)
#endif

#if HAVE_LIMB_LITTLE_ENDIAN
#define HTON_LIMB_STORE(dst, limb)  BSWAP_LIMB_STORE (dst, limb)
#endif

#ifndef HTON_LIMB_STORE
#define HTON_LIMB_STORE(dst, limb)                                      \
  do {                                                                  \
    mp_limb_t  __limb = (limb);                                         \
    char      *__p = (char *) (dst);                                    \
    int        __i;                                                     \
    for (__i = 0; __i < GMP_LIMB_BYTES; __i++)                       \
      __p[__i] = (char) (__limb >> ((GMP_LIMB_BYTES-1 - __i) * 8));  \
  } while (0)
#endif