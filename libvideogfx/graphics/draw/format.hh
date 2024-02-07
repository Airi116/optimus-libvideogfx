
#/*********************************************************************
  libvideogfx/graphics/draw/format.hh

  purpose:
    Functions for extending borders, change format of bitmaps...

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
    04/Jul/2000 - Dirk Farin - bitmap format conversion and helpers
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

#ifndef LIBVIDEOGFX_GRAPHICS_DRAW_BMFORMAT_HH
#define LIBVIDEOGFX_GRAPHICS_DRAW_BMFORMAT_HH

#include <libvideogfx/graphics/datatypes/bitmap.hh>
#include <libvideogfx/graphics/datatypes/image.hh>
#include <libvideogfx/graphics/datatypes/primitives.hh>
#include <libvideogfx/graphics/draw/blit.hh>

namespace videogfx {

  /** Create a greyscale image from a bitmap. */
  template <class T> Image<T> MakeImage(Bitmap<T>&);
  template <class T> const Image<T> MakeImage(const Bitmap<T>&);

  // --------- bitmap type conversion ----------

  template <class A,class B> void ConvertBitmap(Bitmap<B>& dst,const Bitmap<A>& src);
  void PixelDifferenceToPixel(Bitmap<Pixel>& dst,const Bitmap<int16>& src);

  template <class T> void Crop(Image<T>& dest, const Image<T>& src,
			       int left, int right, int top, int bottom);

  /** Enlarge the bitmap by adding additional pixel rows/columns around it. The reference coordinate
      system stays the same. Hence, when adding to the left or the top, the origin moves into the image
      area and X/YOffset will be greater than 0. */
  template <class T> void Enlarge(Bitmap<T>& dest, const Bitmap<T>& src,
				  int addleft, int addtop, int addright, int addbottom,T bkgcolor);
  template <class T> void Enlarge(Image<T>& dest, const Image<T>& src,
				  int addleft, int addtop, int addright, int addbottom,Color<T> bkgcolor);


  // -----------------------------------------------------------------------------------------
  // --------------------------------- implementation ----------------------------------------