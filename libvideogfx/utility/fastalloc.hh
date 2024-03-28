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
    but WITHOUT ANY WARRANTY; without ev