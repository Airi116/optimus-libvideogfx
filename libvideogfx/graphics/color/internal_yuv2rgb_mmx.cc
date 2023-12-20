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

#include "libvideogfx/graphics/color/internal_yuv2rgb_mmx.hh"
using namespace videogfx;  // for "uint64" declaration

#include <iostream>
using namespace std;


/* There is a very strange bug (most probably in the dynamic linker) which causes
   a seg-fault at program start when the following variables are declared static.
*/

uint64 UVoffset   = 0x0080008000800080LL;     //   0  4x  128   // UV offs
uint64 Yoffset    = 0x1010101010101010LL;     //   8  8x   16   // Y offs
uint64 Cb2Rfact   = 0x0066006600660066LL;     //  16  4x  102 =  409/4         // Cb  ->R
uint64 CbCr2Gfact = 0x0034001900340019LL;     //  24  2x (52 25) = 208/4 100/4 //