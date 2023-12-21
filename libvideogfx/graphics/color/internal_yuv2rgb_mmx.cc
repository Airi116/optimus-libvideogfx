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
uint64 CbCr2Gfact = 0x0034001900340019LL;     //  24  2x (52 25) = 208/4 100/4 // CbCr->G
uint64 Cb2Bfact   = 0x0081008100810081LL;     //  32  4x  129 =  516/4         // Cb  ->B
uint64 Yfact      = 0x004A004A004A004ALL;     //  40  4x   74 =  298/4         // Y mul
uint64 shift6bit  = 0x0000000000000006LL;     //  40  4x   74 =  298/4         // Y mul

uint64 tmp_cr, tmp_rimpact, tmp_rimpact2, tmp_gimpact, tmp_bimpact, tmp_bimpact2;



namespace videogfx {

  bool i2r_16bit_mmx::s_CanConvert(const Image<Pixel>& img,const RawRGBImageSpec& spec)
  {
    if (spec.dest_width || spec.upscale_factor || spec.downscale_factor) return false;
    if (spec.bits_per_pixel != 16) return false;
    if (!spec.little_endian) return false;

    ImageParam param = img.AskParam();

    if (param.colorspace != Colorspace_YUV) return false;
    if (param.chroma != Chroma_420) return false;

    if ((param.width & 7) != 0) return false;

    int w = (param.width+7) & ~7;
    if (spec.bytes_per_line < 2*w) return false;