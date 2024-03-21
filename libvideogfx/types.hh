/********************************************************************************
  types.hh

  purpose:
    Basic type declarations and commonly used macros, inline functions.

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
    03/Nov/1999 - Dirk Farin - New constants: BoolPixel_*
 ********************************************************************************
    LibVideoGfx - video processing library
    Copyright (C) 2002  Dirk Farin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ********************************************************************************/

#ifndef LIBVIDEOGFX_TYPES_HH
#define LIBVIDEOGFX_TYPES_HH

#if defined(__GCC__) && (__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 5)
#include <stdint.h>
#else
#include <cstdint>
#endif

#include <cmath>

namespace videogfx {

  typedef uint64_t uint64;
  typedef  int64_t  int64;
  typedef uint32_t uint32;
  typedef  int32_t  int32;
  typedef uint16_t uint16;
  typedef  int16_t  int16;
  typedef uint8_t  uint8;
  typedef  int8_t   int8;

  typedef uint32 uint31;
  typedef uint32 uint30;
  typedef uint32 uint29;
  typedef uint32 uint28;
  typedef uint32 uint27;
  typedef uint32 uint26;
  typedef uint32 uint25;
  typedef uint32 uint24;
  typedef uint32 uint23;
  typedef uint32 uint22;
  typedef uint32 uint21;
  typedef uint32 uint20;
  typedef uint32 uint19;
  typedef uint32 uint18;
  typedef uint32 uint17;

  typedef uint16 uint15;
  