/********************************************************************************
  libvideogfx/utility/bitstream/bitreader.hh

  purpose:
   Implements bit-access to an externally provided memory buffer.

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   03/Jul/2000 - Dirk Farin
     - new method: SkipBitsFast()
   20/Jun/2000 - Dirk Farin
     - completely new implementation
   30/Sep/1999 - Dirk Farin
     - integrated from old DVDview into ULib
   26/Dec/1998 - Dirk Farin
     - made most methods inline
     - new method: "AskBitsLeft()"
   23/Dec/1998 - Dirk Farin
     - first implementation, not working yet
 ********************************************************************************
    LibVideoGfx - video processing library
    Copyright (C) 2002  Dirk Farin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    versi