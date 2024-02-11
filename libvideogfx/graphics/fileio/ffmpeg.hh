/********************************************************************************
  $Header$

    Read file using the FFMPEG library.
 ********************************************************************************
    Copyright (C) 2011 Dirk Farin (see the README for complete list of authors)

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

#ifndef LIBVIDEOGFX_GRAPHICS_FILEIO_FFMPEG_HH
#define LIBVIDEOGFX_GRAPHICS_FILEIO_FFMPEG_HH

#include <libvideogfx/graphics/datatypes/image.hh>

struct AVFormatContext;
struct AVInputFormat;
struct AVCodecContext; 
struct AVCodec;
struct AVFrame;

namespace videogfx
{
  class FileReader_FFMPEG
  {
  public:
    FileReader_FFMPEG();
    ~FileReader_FFMPEG();

    bool Open(const char* filedescr);
    void Close();

    bool IsEOF() const { return m_eof; }
    bool ReadImage(Image<Pixel>&);
    void SkipToImage(int nr);

    int AskWidth() const { return w; }
    int AskHeight() const { return h; }
    float getFPS() const { return fps; }

  private:
    struct AVFormatContext* formatCtx;
    struct AVCodecContext* codecCtx;
    int                   videoStreamIdx;
    struct AVCodec* codec;
    struct AVFrame* frame;
    struct AVFrame* frameRGB;
    uint8* buffer;
    int  w,h;
    float fps;

    int  m_preloadFrameNr;
    bool m_eof;

    bool m_isInSync;

    void convertRGBBuffer(Image<Pixel>& img);
    void preload(int framenr);
  };
}

#endif
