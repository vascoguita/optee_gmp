/* mpz_inp_str(dest_integer, stream, base) -- Input a number in base
   BASE from stdio stream STREAM and store the result in DEST_INTEGER.

   OF THE FUNCTIONS IN THIS FILE, ONLY mpz_inp_str IS FOR EXTERNAL USE, THE
   REST ARE INTERNALS AND ARE ALMOST CERTAIN TO BE SUBJECT TO INCOMPATIBLE
   CHANGES OR DISAPPEAR COMPLETELY IN FUTURE GNU MP RELEASES.

Copyright 1991, 1993, 1994, 1996, 1998, 2000-2003, 2011-2013 Free Software
Foundation, Inc.

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
#include <ctype.h>
#include "gmp-impl.h"
#include "longlong.h"

#define digit_value_tab __gmp_digit_value_tab