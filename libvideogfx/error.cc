
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

#include "error.hh"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <iostream>

namespace videogfx {
  using namespace std;

  static class MessageDisplay_cerr : public MessageDisplay
  {
  public:
    void ShowMessage(ErrorSeverity severity,const char* text)
    {
      cout.flush(); /* Flush program output. Do this for better sync of
                       debug and error display. */

      switch(severity)
        {
          //case ErrSev_Empty:     cerr << "EMPTY-ERROR ! This should not occur !\n"; exit(10); break;
        case ErrSev_Note:      cerr << "Note: ";    break;
        case ErrSev_Warning:   cerr << "Warning: "; break;
        case ErrSev_Error:     cerr << "Error: ";   break;
        case ErrSev_Critical:  cerr << "CRITICAL ERROR: "; break;
        case ErrSev_Assertion: cerr << "ASSERTION FAILED: "; break;
        }

      cerr << text << endl;
    }

    void ShowMessage(const Excpt_Base& e)
    {
      char buf[1000]; // TODO: replace me
      e.GetText(buf,1000);

      ShowMessage(e.m_severity , buf);
    }
  } msgdisplay_cerr;


  void MessageDisplay::Show(ErrorSeverity sev,const char* text)
  {
    assert(std_msgdisplay);
    std_msgdisplay->ShowMessage(sev,text);
  }

  void MessageDisplay::Show(const Excpt_Base& excpt)
  {
    assert(std_msgdisplay);
    std_msgdisplay->ShowMessage(excpt);
  }

  void MessageDisplay::SetStandardDisplay(MessageDisplay* disp)
  {
    assert(disp);
    std_msgdisplay = disp;
  }

  MessageDisplay* MessageDisplay::std_msgdisplay = &msgdisplay_cerr;





  Excpt_Base::Excpt_Base(ErrorSeverity sev)
    : m_severity(sev)
  {
    assert(m_severity != ErrSev_Note);
  }


