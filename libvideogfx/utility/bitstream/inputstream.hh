/********************************************************************************
  utility/bitstream/inputstream.hh
    Generic input stream base class.

  purpose:

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   23/Feb/2002 - Dirk Farin
     - first implementation
 ********************************************************************************
    Copyright (C) 2002  Dirk Farin

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

#ifndef LIBVIDEOGFX_UTILITY_BITSTREAM_INPUTSTREAM_HH
#define LIBVIDEOGFX_UTILITY_BITSTREAM_INPUTSTREAM_HH

#include <libvideogfx/types.hh>

namespace videogfx {

  class InputStream
  {
  public:
    InputStream();
    virtual ~InputStream();

    /* Fill buffer 'mem' with up to 'maxlen' bytes. Number of bytes written is
       returned. This may be less than requested but never less than 'minlen'.
       the full buffer is filled. Only if the input does not contain enough
       bytes to fill at least 'minlen' bytes into the buffer, less bytes are returned. */
    uint32 FillBuffer(uint8* mem,uint32 maxlen,uint32 minlen=1);

    /* Push back the given bytes to the input queue. You may push back
       as many bytes as you want and call this function as often as you like to. */
    void   Pushback(uint8* mem,int n_bytes);

    virtual bool   IsEOF() const = 0;

    /* If the stream is of finite length, you may call AskStreamLength().
       It is however not guaranteed that the size of the stream will not
       grow any more.

 