/*********************************************************************
  fileio/raw.hh

  purpose:
    read and write raw images

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   11/Aug/2007 - Dirk Farin - first implementation
 ********************************************************************************
    LibVideoGfx - video processing library
    Copyright (C) 2007  Dirk Farin

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


#ifndef LIBVIDEOGFX_GRAPHICS_FILEIO_RAW_HH
#define LIBVIDEOGFX_GRAPHICS_FILEIO_RAW_HH

#include <fstream>
#include <iostream>

#include <libvideogfx/graphics/datatypes/image.hh>

namespace videogfx {

  class RawImageReader
  {
  public:
    RawImageReader();

    void SetSize(int w,int h) { d_w=w; d_h=h; }
    void SetBitsPerPixels(int bpp) { d_bpp=bpp; }
    void SetHeaderSkip(int nbytes) { d_header_bytes=nbytes; }
    void SetLittleEndian(bool flag=true) { d_little_endia