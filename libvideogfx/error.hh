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

    This librar