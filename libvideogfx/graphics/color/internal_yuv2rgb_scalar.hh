/*********************************************************************
  libvideogfx/graphics/color/internal_yuv2rgb_scalar.hh

  purpose:
    Transform YUV data into 16bit true color RGB raw data.
    Every bit organization in a 16bit field and endianess
    translation is supported.

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
    08/Aug/1999 - Dirk Farin - code imported from DVDview and
                               slightly modified
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

    You should have received a copy of