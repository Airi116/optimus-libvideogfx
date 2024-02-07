/*********************************************************************
  libvideogfx/graphics/draw/pixelops.hh

  purpose:
    Single pixel operations that are applied to full bitmaps.

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   30/Jan/2002 - Dirk Farin - adapted the interface
   27/Apr/2001 - Dirk Farin - first implementation
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

#ifndef LIBVIDEOGFX_GRAPHICS_DRAW_PIXELOPS_HH
#define LIBVIDEOGFX_GRAPHICS_DRAW_PIXELOPS_HH

#include <libvideogfx/graphics/datatypes/bitmap.hh>

namespace videogfx {

  template <class T> void MakeAbsolute(Bitmap<T>&);
  template <class T> void Inverse(Bitmap<T>&,T maxval);
  template <class T> void AddValues(Bitmap<T>& dest,const Bitmap<T>& a,const Bitmap<T>& b);
  template <class T> void Add(Bitmap<T>& dest,T val);
  template <class T> void HalfPlusOffset(Bitmap<T>& bm,T offset); // p = p/2+offset

  /* dest = a-b */
  template <class T> void Ab