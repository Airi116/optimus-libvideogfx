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
  template <class Pel> void CopyToNew(Bitmap<Pel>& dst,const B