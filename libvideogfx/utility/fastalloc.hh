/********************************************************************************
  $Header$

  purpose:
    Very fast memory allocation.

  notes:
   - It is suggested that you create a separate MemoryAllocator object
     for each purpose of allocation as the allocator may generate statistics
     on how large the allocated memory area usually are. This may be needed
     for optimal performance.

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   28/Sep/1999 - Dirk Farin
     - first implementation based on old DVDView's alloc.cc
 ********************************************************************************
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

#ifndef LIBVIDEOGFX_UTILITY_FASTALLOC_HH
#define LIBVIDEOGFX_UTILITY_FASTALLOC_HH

#include <libvideogfx/types.hh>

namespace videogfx {

  class MemoryAllocator
  {
  public:
    MemoryAllocator(int MinimumMemorySize=1000,int PoolSize=10);
    ~MemoryAllocator();

    void* Alloc(int size,int* realsize=NULL);
    void  Free(void*);

    void  ResetPool(); // delete all the memory in the pool

  private:
    int** d_Pool;
    int     d_nAreasInPool;
    int     d_PoolSize;
    int     d_MinMemSize;
  };
}

#endif
