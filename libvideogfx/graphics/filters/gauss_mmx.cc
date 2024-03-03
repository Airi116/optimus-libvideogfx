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
  .    2*  I