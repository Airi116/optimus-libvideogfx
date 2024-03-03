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

#include "libvideogfx/graphics/filters/gauss_mmx.hh"
#include "libvideogfx/arch/mmx.h"
#include <assert.h>

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

  void LowPass_3x3Gauss_MMX(Bitmap<Pixel>& destbm, const Bitmap<Pixel>& srcbm)
  {
    assert(false); // "Use of \"LowPass_3x3Gauss_MMX()\" is deprecated. Use LowPass_Binormial_Decimate() instead"

    destbm.Create((srcbm.AskWidth()+1)/2,(srcbm.AskHeight()+1)/2 ,8);

    const Pixel*const* src = srcbm.AskFrame();
    Pixel*const* dst = destbm.AskFrame();
    int w = srcbm.AskWidth();
    int h = srcbm.AskHeight();

    uint8* line = new Pixel[w+32];
    uint8* l = &line[16];

    uint64 hb = 0xFF00FF00L; hb = hb | (hb<<32);  // do it the awkward way to be compilable with Windows
    uint64 lb = 0x00FF00FFL; lb = lb | (lb<<32);

    for (int y=0;y<h;y+=2)
      {
	/