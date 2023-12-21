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

    if (param.height & 1) return false;

    return true;
  }


  void i2r_16bit_mmx::Transform(const Image<Pixel>& img,uint8* mem,int firstline,int lastline)
  {
    uint64 constants[20];

    const uint32 rmask=d_spec.r_mask;
    const uint32 gmask=d_spec.g_mask;
    const uint32 bmask=d_spec.b_mask;

    uint32 rshift,gshift,bshift;

    rshift = d_spec.r_shift;  rshift -= 8-d_spec.r_bits;  rshift -= 8;  rshift = -rshift;
    gshift = d_spec.g_shift;  gshift -= 8-d_spec.g_bits;  gshift -= 8;  gshift = -gshift;
    bshift = d_spec.b_shift;  bshift -= 8-d_spec.b_bits;  bshift -= 8;  bshift = -bshift;


    // ---------------------------------------

    constants[0] = 0x0080008000800080LL;     //   0  4x  128   // UV offs
    constants[1] = 0x1010101010101010LL;     //   8  8x   16   // Y offs
    constants[2] = 0x0066006600660066LL;     //  16  4x  102 =  409/4         // Cb  ->R
    constants[3] = 0x0034001900340019LL;     //  24  2x (52 25) = 208/4 100/4 // CbCr->G
    constants[4] = 0x0081008100810081LL;     //  32  4x  129 =  516/4         // Cb  ->B
    constants[5] = 0x004A004A004A004ALL;     //  40  4x   74 =  298/4         // Y mul

    //  6 tmp  0        //  48
    //  7 tmp  8        //  56
    //  8 tmp 16        //  64
    //  9 tmp 24        //  72
    // 10 tmp 32        //  80
    // 11 tmp 40        //  88

    static uint64 bitsconsts[9] =
    {
      0,
      0xfefefefefefefefeLL,     // 1 bit-Mask
      0xfcfcfcfcfcfcfcfcLL,     // 2 bit-Mask
      0xf8f8f8f8f8f8f8f8LL,     // 3 bit-Mask
      0xf0f0f0f0f0f0f0f0LL,     // 4 bit-Mask
      0xe0e0e0e0e0e0e0e0LL,     // 5 bit-Mask
      0xc0c0c0c0c0c0c0c0LL,     // 6 bit-Mask
      0x8080808080808080LL,     // 7 bit-Mask
      0
    };

    constants[12] = bitsconsts[d_spec.r_bits];   //  96
    constants[13] = bitsconsts[d_spec.g_bits];   // 104
    constants[14] = bitsconsts[d_spec.b_bits];   // 112
    constants[15] = (8-d_spec.r_bits)+6;         // 120
    constants[16] = (8-d_spec.g_bits)+6;         //