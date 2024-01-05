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
          boundary conditions,
      <li>multiple views into a single bitmap, to use a small part
          of a bitmap like a bitmap of its own, or to have acces to
          interlaced fields just like full-resolution frames (which
          for example simplifies MPEG field-MC),
      <li>ability to wrap fixed memory ranges (like your gfx-card
          memory) into a Bitmap (not written yet).
      </ul>

      Internally, Bitmap is only the \em envelope class to the \em letter
      class BitmapProvider, which contains the actual data. When a bitmap
      is copied, the data is not copied! Instead, copying will result in
      a second Bitmap-envelope object using the same bitmap data. Changes
      made to one bitmap will hence also influence the other. If you want
      to make an independent copy of your bitmap, use Clone() instead.

      Usually, a Bitmap is created in main memory by specifying its size
      and optionally alignment and a surrounding border.
      For special purpose applications, BitmapProviders can be defined which
      wrap the Bitmap-API around existing memory blocks.

      When creating bitmaps in main memory, optionally to the bitmap size, a
      border and horizontal, vertical alignments can be specified. The border
      is a safety memory around the actual bitmap data. I.e., you may access
      bitmap content if indexes in the range \f$[-border; width+border[\f$.
      An alignment rounds the bitmap up to a multiple of the alignment factor
      (but not necessarily the next multiple). Be warned that you may get
      differing values for border and alignment back from the bitmap class.
      However, these will never be smaller than the values you specified.
      Horizontal border sizes are added before doing the alignment. Hence,
      the total width and stride (number of data elements, usually bytes,
      between two lines) will be a multiple of the horizontal alignment.
      \image latex 