/*********************************************************************
  libvideogfx/graphics/fileio/unified_loader.hh

  purpose:
    Makes the loading of files much easier by providing a unified
    interface to loading any kind of images and sequences.
    The input can even be modified by a set of filters like
    deinterlacing or cropping. A further nice feature is to
    save parts of the input-definition pipeline as a macro,
    so that sequences can be accessed very easily.

  usage:
    =macro -- macro expansion (define in ~/.libvideogfxrc)
    range:s:l -- only show 'l' pictures, starting from 's'
    length:l -- sequence length is 'l'
    start:s -- first frame at 's'
    decimate:f -- only show every f'th frame (f=3 -> 2,5,8,...)
    resize:w:h -- resize image
    crop:l:r:t:b -- crop away image borders
    quarter -- resize to quarter size (especially useful to deinterlace to CIF)
    alpha:n -- add alpha-channel file 'n' to the loaded images
    cache -- add a temporary disk-based image cache to allow searching
    rgb -- generate alternating R,G,B images

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   30/Jul/04 - Dirk Farin - first implementation
 ********************************************************************************
    LibVideoGfx - video processing library
    Copyright (C) 2004  Dirk Farin

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
    Foundation, Inc., 59 Tem