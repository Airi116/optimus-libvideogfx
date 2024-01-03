/********************************************************************************
  $Header$
*/
/*
  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
    23/Jan/2002 - Dirk Farin - Complete reimplementation based on old Bitmap type.
    02/Jun/1999 - Dirk Farin - First implementation based on DVDview code.
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

#ifndef LIBVIDEOGFX_GRAPHICS_BASIC_BITMAP_HH
#define LIBVIDEOGFX_GRAPHICS_BASIC_BITMAP_HH

#include <libvideogfx/types.hh>
#include <libvideogfx/utility/math.hh>
#include <string.h>
#include <algorithm>
#include <assert.h>

namespace videogfx {

  template <class Pel> class BitmapProvider;

  /** Bitmap data-type definition.
      A Bitmap can be used for any two-dimensional image data using
      arbitrary types for the pixels. In that sense, a Bitmap is nothing
      but a two-dimensional array. What makes the Bitmap special is
      <ul>
      <li>automatic reference-counter based memory handling, making
          copying efficient,
      <li>transparent alignment of bitmap sizes to integer multiples
          of some constants (e.g., 16 pixels to simplify MPEG coders),
      <li>transparent border around the bitmap to allow algorithms,
          that consider a context around a current pixel, to ignore
        