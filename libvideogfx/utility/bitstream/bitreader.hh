/********************************************************************************
  libvideogfx/utility/bitstream/bitreader.hh

  purpose:
   Implements bit-access to an externally provided memory buffer.

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   03/Jul/2000 - Dirk Farin
     - new method: SkipBitsFast()
   20/Jun/2000 - Dirk Farin
     - completely new implementation
   30/Sep/1999 - Dirk Farin
     - integrated from old DVDview into ULib
   26/Dec/1998 - Dirk Farin
     - made most methods inline
     - new method: "AskBitsLeft()"
   23/Dec/1998 - Dirk Farin
     - first implementation, not working yet
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

#ifndef LIBVIDEOGFX_UTILITY_BITSTREAM_BITREADER_HH
#define LIBVIDEOGFX_UTILITY_BITSTREAM_BITREADER_HH

#include "libvideogfx/types.hh"
#include "libvideogfx/utility/bitstream/inputstream.hh"

namespace videogfx {

  class BitReader
  {
  public:
    BitReader(InputStream& istr);
    BitReader(const uint8* buffer,uint32 len);
    ~BitReader();

    inli