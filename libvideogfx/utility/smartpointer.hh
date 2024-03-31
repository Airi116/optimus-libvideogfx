/********************************************************************************
  $Header$

  purpose:
    Generic smart pointer class.

    SP is a "smart pointer" class with reference counting. You can
    copy smart pointers or pass them to subroutines.

    SSP is a "simple smart pointer" which does not support copying.
    It is useful for automatic object destruction only.

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   09/May/2002 - Dirk Farin
     - 