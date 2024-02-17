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

#include <libvideogfx/graphics/fileio/imagesink.hh>
#include <libvideogfx/graphics/fileio/jpeg.hh>
#include <libvideogfx/graphics/fileio/ppm.hh>
#include <libvideogfx/graphics/fileio/png.hh>
#include <libvideogfx/graphics/color/colorspace.hh>
#include <stdio.h>

namespace videogfx {
  using namespace std;

  ImageSink_Save::ImageSink_Save(ImageFileFormat fmt)
    : d_filename_template(NULL),
      d_curr_nr(0)
  {
    SetFileFormat(fmt);
    SetFilename("img",true);
  }

  ImageSink_Save::~ImageSink_Save()
  {
    if (d_filename_template) delete[] d_filename_template;
  }

  bool ImageSink_Save::IsFormatSupported(ImageFileFormat f)
  {
    switch (f)
      {
      case Format_PPM:
      case Format_PGM:
	return true;
	break;
      case Format_JPEG:
	return JPEG_Supported();
	break;
      case Format_PNG:
	return PNG_Supported();
	break;
      }

    assert(0);
    return false;
  }

  void ImageSink_Save::SetFilename(const char* tmpl, bool autosuffix)
  {
    if (d_filename_template) delete[] d_filename_template;

    int len = strlen(tmpl)+(autosuffix ? 4 : 0)+1;
    d_filename_template = new char[len];

    strcpy(d_filename_template,tmpl);
    d_autosuffix = autosuffix;
  }

  void ImageSink_Save::SendImage(const Image<Pixel>& img)
  {
    