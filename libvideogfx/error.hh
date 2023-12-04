/********************************************************************************
  error.hh

  purpose:
    Error handling stuff: exception handling and error message display.

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   29/Sep/1999 - Dirk Farin
    - ShowMessage() can now be given a Excpt_Base argument. This allows
      the MessageDisplay to selectively enable/disable the display of
      some error classes.
   20/Sep/1999 - Dirk Farin
    - complete cleanup, ShowNote() removed
   03/Jan/1999 - Dirk Farin
    - ShowNote()
   28/Dec/1998 - Dirk Farin
    - new implementation based on exceptions
   15/Nov/1998 - Dirk Farin
    - first implementation
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

#ifndef LIBVIDEOGFX_ERROR_HH
#define LIBVIDEOGFX_ERROR_HH

// We need: __ASSERT_FUNCTION
#  include <assert.h>

namespace videogfx {

  /* Severity 