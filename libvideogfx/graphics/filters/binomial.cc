/********************************************************************************
    LibVideoGfx - video processing library
    Copyright (C) 2002  Dirk Farin, Gerald Kuehne

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

#include <libvideogfx/graphics/filters/binomial.hh>
#include <libvideogfx/arch/cpu.hh>
#include "config.h"

#if ENABLE_MMX
#  include "libvideogfx/arch/mmx.h"
#endif


#include <iostream>
  using namespace std;


/*
  Flow graph for row transform:

  MEM:      I00 I01 I02 I03 I04 I05 I06 I07      8bit      I08 I09 I10 I11 I12 I13 I14 I15
  Reg:      I07 I06 I05 I04 I03 I02 I01 I00      8bit                   .....
  .        /  deinterlace / \  deinterlace \
  .        I07 I05 I03 I01   I06 I04 I02 I00    16bit

  .       -----------------------------------

  .        I13 I11 I09 I07   I05 I03 I01 I-1    16bit
  .    2*  I14 I12 I10 I08   I06 I04 I02 I00     ...
  .        I15 I13 I11 I09   I07 I05 I03 I01     ...
  .     =  O07 O06 O05 O04   O03 O02 O01 O00    16bit
  .        \              \ /              /
  Reg:      O07 O06 O05 O04 O03 O02 O01 O00      8bit
  MEM:      O00 O01 O02 O03 O04 O05 O06 O07      8bit
 */

namespace videogfx {

#if ENABLE_MMX
  static void LowPass_Binomial_Downsample_MMX (Bitmap<Pixel>& destbm, const Bitmap<Pixel>& srcbm)
  {
    destbm.Create((srcbm.AskWidth()+1)/2,(srcbm.AskHeight()+1)/2 ,8);

    const Pixel*const* src = srcbm.AskFrame();
    Pixel*const* dst = destbm.AskFrame();
    int w = srcbm.AskWidth();
    int h = srcbm.AskHeight();

    uint8* line = new Pixel[w+32];
    uint8* l = &line[16];

    uint64 hb = 0xFF00FF00L; hb = hb | (hb<<32);  // do it the awkward way to be compilable with Windows
    uint64 lb = 0x00FF00FFL; lb = lb | (lb<<32);
    uint64 a2 = 0x00020002L; a2 = a2 | (a2<<32);

    for (int y=0;y<h;y+=2)
      {
	// Column transform

	pxor_r2r(mm0,mm0);

	for (int x=0;x<w;x+=8)
	  {
	    uint64* sp_m1 = (uint64*)&src[y-1][x];
	    uint64* sp_0  = (uint64*)&src[y  ][x];
	    uint64* sp_p1 = (uint64*)&src[y+1][x];
	    uint64* dp    = (uint64*)&l[x];

	    // unpack 8 pixels of (y-1) to 16bit in (mm1,mm2)

	    movq_m2r(*sp_m1,mm1);
	    movq_r2r(mm1,mm2);
	    punpckhbw_r2r(mm0,mm1);
	    punpcklbw_r2r(mm0,mm2);

	    // unpack 8 pixels of (y) to 16bit and add twice to (mm1,mm2)

	    movq_m2r(*sp_0,mm3);
	    movq_r2r(mm3,mm4);
	    punpckhbw_r2r(mm0,mm3);
	    punpcklbw_r2r(mm0,mm4);
	    paddw_r2r(mm3,mm1);
	    paddw_r2r(mm4,mm2);
	    paddw_r2r(mm3,mm1);
	    paddw_r2r(mm4,mm2);

	    // unpack 8 pixels of (y+1) to 16bit and add to (mm1,mm2)

	    movq_m2r(*sp_p1,mm5);
	    movq_r2r(mm5,mm6);
	    punpckhbw_r2r(mm0,mm5);
	    punpcklbw_r2r(mm0,mm6);
	    paddw_r2r(mm5,mm1);
	    paddw_r2r(mm6,mm2);

	    // divide by 4

	    paddw_m2r(a2,mm1);
	    paddw_m2r(a2,mm2);

	    psrlw_i2r(2,mm1);
	    psrlw_i2r(2,mm2);

	    // store to line buffer

	    packuswb_r2r(mm1,mm2);

	    movq_r2m(mm2,*dp);
	  }

	l[-1] = l[0];
	l[w]  = l[w-1];

	// Row 