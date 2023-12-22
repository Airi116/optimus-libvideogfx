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
    constants[16] = (8-d_spec.g_bits)+6;         // 128
    constants[17] = (8-d_spec.b_bits)+6;         // 136
    constants[18] = d_spec.r_shift-8;            // 144
    constants[19] = d_spec.g_shift;              // 152



    // --------- TRANSFORM -----------

    ImageParam param = img.AskParam();

    assert(param.chroma==Chroma_420);

    assert((firstline%2)==0);

    const Pixel*const* pix_y  = img.AskFrameY();
    const Pixel*const* pix_cb = img.AskFrameU();
    const Pixel*const* pix_cr = img.AskFrameV();

    int chr_w, chr_h;

    param.AskChromaSizes(chr_w,chr_h);

    const int w = param.width;

    int yskip = 2*(pix_y [1]-pix_y [0]) - w;
    int cskip =   (pix_cb[1]-pix_cb[0]) - w/2;
    int mskip = 2*d_spec.bytes_per_line - 2*w;

    const uint8*  yptr1 = (uint8*)pix_y[firstline];
    const uint8*  yptr2 = (uint8*)pix_y[firstline+1];
    const uint8*  cbptr = (uint8*)pix_cb[firstline/2];
    const uint8*  crptr = (uint8*)pix_cr[firstline/2];
    uint8* membuf_a = ((uint8*)(mem));
    uint8* membuf_b = ((uint8*)(mem))+d_spec.bytes_per_line;

    for (int y=firstline;y<=lastline;y+=2)
      {
	for (int x=0;x<w;x+=8)
	  {
	    __asm__ __volatile__
	      (
	       "movd        (%1),%%mm1\n\t"   // 4 Cb-Werte nach mm1
	       " pxor       %%mm0,%%mm0\n\t"  // mm0=0
	       "movd        (%2),%%mm2\n\t"   // 4 Cr-Werte nach mm2
	       " punpcklbw  %%mm0,%%mm1\n\t"  // Cb-Werte in mm1 auf 16bit Breite bringen
	       "psubw       (%3),%%mm1\n\t"   // Offset 128 von Cb-Werten abziehen
	       " punpcklbw  %%mm0,%%mm2\n\t"  // Cr-Werte in mm2 auf 16bit Breite bringen
	       "psubw       (%3),%%mm2\n\t"   // Offset 128 von Cr-Werten abziehen
	       " movq       %%mm1,%%mm3\n\t"  // Kopie von Cb-Werten nach mm3
	       "movq        %%mm1,%%mm5\n\t"  // ... und nach mm5
	       " punpcklwd  %%mm2,%%mm1\n\t"  // in mm1 ist jetzt: LoCr1 LoCb1 LoCr2 LoCb2
	       "pmaddwd     24(%3),%%mm1\n\t" // mm1 mit CbCr-MulAdd -> LoGimpact1 LoGimpact2
	       " punpckhwd  %%mm2,%%mm3\n\t"  // in mm3 ist jetzt: HiCr1 HiCb1 HiCr2 HiCb2
	       "pmaddwd     24(%3),%%mm3\n\t" // mm3 mit CbCr-MulAdd -> HiGimpact1 HiGimpact2
	       "movq        %%mm2,48(%3)\n\t" // mm2 sichern (Cr)
	       "movq        (%0),%%mm6\n\t"   // 8 Y-Pixel nach mm6
	       "psubusb     8(%3),%%mm6\n\t"  // Y -= 16
	       " packssdw   %%mm3,%%mm1\n\t"  // mm1 enthaelt nun 4x G-Impact
	       "movq        %%mm6,%%mm7\n\t"  // Y-Pixel nach mm7 kopieren
	       " punpcklbw  %%mm0,%%mm6\n\t"  // 4 low Y-Pixel nach mm6
	       "pmullw      40(%3),%%mm6\n\t" // ... diese mit Ymul multiplizieren
	       " punpckhbw  %%mm0,%%mm7\n\t"  // 4 high Y-Pixel nach mm7
	       "pmullw      40(%3),%%mm7\n\t" // ... diese mit Ymul multiplizieren
	       " movq       %%mm1,%%mm4\n\t"  // G-Impact nach mm4
	       "movq        %%mm1,56(%3)\n\t" // G-Impact sichern
	       " punpcklwd  %%mm1,%%mm1\n\t"  // beide low G-Impacts verdoppeln
	       "movq        %%mm6,%%mm0\n\t"  // 4 low Y-Pixel nach mm0
	       " punpckhwd  %%mm4,%%mm4\n\t"  // beide high G-Impacts verdoppeln
	       "movq        %%mm7,%%mm3\n\t"  // 4 high Y-Pixel nach mm3
	       " psubw      %%mm1,%%mm6\n\t"  // 4 low G in mm6 berechnen
	       "psraw       128(%3),%%mm6\n\t"// G-Werte in mm6 in richtige Position bringen
	       " psubw      %%mm4,%%mm7\n\t"  // 4 high G in mm7 berechnen
	       "movq        %%mm5,%%mm2\n\t"  // 4 Cr-Werte nach mm2
	       " punpcklwd  %%mm5,%%mm5\n\t"  // beide low Cr-Impacts verdoppeln
	       "pmullw      32(%3),%%mm5\n\t" // 4 low B-Impacts berechnen
	       " punpckhwd  %%mm2,%%mm2\n\t"  // beide high Cr-Impacts verdoppeln
	       "psraw       128(%3),%%mm7\n\t"// G-Werte in mm7 in richtige Position bringen
	       " pmullw     32(%3),%%mm2\n\t" // 4 high B-Impacts berechnen
	       "packuswb    %%mm7,%%mm6\n\t"  // G-Werte in mm6 zusammenfassen
	       "movq        %%mm5,64(%3)\n\t" // 4 low B-Impacts sichern
	       " paddw      %%mm0,%%mm5\n\t"  // 4 low B in mm5 berech