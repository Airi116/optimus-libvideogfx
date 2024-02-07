/********************************************************************************
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

#include "libvideogfx/graphics/draw/format.hh"

namespace videogfx {

  void PixelDifferenceToPixel(Bitmap<Pixel>& dst,const Bitmap<int16>& src)
  {
    int w=src.AskWidth();
    int h=src.AskHeight();

    dst.Create(w,h);

    const int16*const* ap = src.AskFrame();
    Pixel*const* bp = dst.AskFrame();

    for (int y=0;y<h;y++)
      for (int x=0;x<w;x++)
	bp[y][x] = (Pixel)(ap[y][x]/2+128);
  }

}
