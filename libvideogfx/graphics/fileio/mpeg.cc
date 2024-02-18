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

#include "libvideogfx/graphics/fileio/mpeg.hh"
#include <stdlib.h>
#include <algorithm>

namespace videogfx {
  using namespace std;

  FileReader_MPEG::FileReader_MPEG()
    : d_fh(NULL),
      d_next_framenr(0),
      d_image_cache_full(false)
  {
  }

  FileReader_MPEG::~FileReader_MPEG()
  {
    if (d_fh) pclose(d_fh);
  }

  void FileReader_MPEG::Open(const char* filename)
  {
    if (d_fh) pclose(d_fh);

    char buf[100];
    sprintf(buf,"dvdview -L -W - %s",filename);
#ifdef _WIN32
    d_fh = popen(buf,"rb");
#else
    d_fh = popen(buf,"r");
#endif
  }

  bool FileReader_MPEG::IsEOF() const
  {
    if (d_image_cache_full)
      return false;

    //d_image_cache_f