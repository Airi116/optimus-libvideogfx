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

#include <libvideogfx/arch/cpu.hh>
#include <libvideogfx/error.hh>
#include <libvideogfx/types.hh>
#include "config.h"

#include <string.h>
#include <fstream>

namespace videogfx {
  using namespace std;

  const char* CPU_Capabilities::ArchName(CPU_Architecture arch)
  {
    switch (arch)
      {
      case CPUArch_X86:     return "intel x86"; break;
      case CPUArch_68k:     return "680x0"; break;
      case CPUArch_PowerPC: return "PowerPC"; break;
      case CPUArch_ARM:     return "ARM"; break;
      case CPUArch_Sparc:   return "Sparc"; break;
      case CPUArch_HPPA:    return "HP-PA"; break;
      default:              return "unknown"; break;
      }
  }



  class CPU_Generic : public CPU_Capabilities
  {
    // nothing special
  };

  // --------------------------------------------------------------------------

#if CPU_x86

#define CPU_CAP_MMX      (1<<0)
#define CPU_CAP_MMXEXT   (1<<1)
#define CPU_CAP