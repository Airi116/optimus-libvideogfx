/*********************************************************************
  libvideogfx/graphics/draw/blit.hh

  purpose:
    Functions for copying images, extracting rectangles, ...

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
    29/Jan/2002 - Dirk Farin - first implementation
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

#ifndef LIBVIDEOGFX_GRAPHICS_DRAW_BLIT_HH
#define LIBVIDEOGFX_GRAPHICS_DRAW_BLIT_HH

#include <libvideogfx/graphics/datatypes/bitmap.hh>
#include <libvideogfx/graphics/datatypes/image.hh>
#include <libvideogfx/graphics/datatypes/primitives.hh>

namespace videogfx {

  /* Copy bitmap "src" into "dst". The destination bitmap will be created to have the
     same size as the source bitmap. If the source bitmap is empty, then the destination
     bitmap content is released.
     If the destination is shared, a new bitmap will be created.
  */
  template <class Pel> void CopyToNew(Bitmap<Pel>& dst,const Bitmap<Pel>& src);

  /* Logically the same as Image.Clone(). However, when the destination image is
     already of the right size and not shared, only the image content is copied
     without allocating new memory. */
  template <class Pel> void CopyToNew(Image<Pel>& dst,const Image<Pel>& src);

  /* Copy bitmap to destination. Destination bitmap must exist and will not
     be decoupled if it is shared. */
  template <class Pel> void Copy(Bitmap<Pel>& dst,const Bitmap<Pel>& src);
  template <class Pel> void Copy(Image<Pel>& dst,const Image<Pel>& src);

  /* Copy a region of one bitmap into another bitmap.
   */
  template <class Pel> void Copy(Bitmap<Pel>& dst,       int dstx0,int dsty0,
				 const Bitmap<Pel>& src, int srcx0,int srcy0, int w,int h);
  template <class Pel> void Copy(Image<Pel>& dst,       int dstx0,int dsty0,
				 const Image<Pel>& src, int srcx0,int srcy0, int w,int h);

  /* Copy the outer 'border' rows and columns of the src into the destination i