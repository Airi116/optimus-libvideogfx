
/********************************************************************************
  $Header$

    Read file using a input pipe through MPlayer
 ********************************************************************************
    Copyright (C) 2004 Dirk Farin (see the README for complete list of authors)

    This program is distributed under GNU Public License (GPL) as
    outlined in the COPYING file that comes with the source distribution.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ********************************************************************************/

#ifndef LIBVIDEOGFX_GRAPHICS_FILEIO_MPLAYER_HH
#define LIBVIDEOGFX_GRAPHICS_FILEIO_MPLAYER_HH

#include "libvideogfx/graphics/fileio/yuv4mpeg.hh"
#include <fstream>
#include <sys/types.h>
#include <signal.h>

namespace videogfx
{
  class FileReader_MPlayer
  {
  public:
    FileReader_MPlayer() : d_filedescr(NULL), mplayer_pid(0), framenr(0) { }
    ~FileReader_MPlayer();

    void Open(const char* filedescr);
    void Close();

    bool IsEOF() const;
    void ReadImage(Image<Pixel>&);
    void SkipToImage(int nr);
  
    int AskWidth() const { return w; }
    int AskHeight() const { return h; }

  private:
    char* d_filedescr;

    std::ifstream d_istr;
    FileReader_YUV4MPEG d_reader;
    int w,h;

    pid_t mplayer_pid;
    int framenr;

    Image<Pixel> d_preload;
  };
}

#endif