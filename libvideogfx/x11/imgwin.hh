
/*********************************************************************
  libvideogfx/x11/imgwin.hh

  purpose:
    X11-wrapper classes that simplify window creation and
    displaying true color images in them.

  notes:

  to do:
    - There seems to be a problem with MultiWindowRefresh. Some
      parts of the image sometimes don't get updated. At the moment
      everything seems to work, though.

  author(s):
   - Dirk Farin, farin@gmx.de

  modifications:
   31/Jul/2000 - Dirk Farin - new function: MultiWindowRefresh
   03/Aug/1999 - Dirk Farin - new class: ImageWindow_Autorefresh_X11
   29/Jul/1999 - Dirk Farin - first implementation
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

#ifndef LIBVIDEOGFX_X11_IMGWIN_HH
#define LIBVIDEOGFX_X11_IMGWIN_HH

#include <X11/Xlib.h>
#include "dispimg.hh"

#include <libvideogfx/graphics/datatypes/image.hh>
#include <libvideogfx/graphics/color/img2raw.hh>

namespace videogfx {

  /* Wrapper class for creating one X11 window with the highest color depth possible.
   */
  class ImageWindow_X11
  {
  public:
    ImageWindow_X11();
    ~ImageWindow_X11(); // Window will be closed in destructor.

    void SetPosition(int x,int y) { d_xpos=x; d_ypos=y; }

    void Create(int w,int h,const char* title,X11Server* server=NULL,Window parent=0);
    void Close();

    Window   AskWindow();
    Display* AskDisplay();

  private:
    bool        d_initialized;

    struct X11SpecificData* d_x11data; // This hides X11 datatypes from global namespace.
    X11ServerConnection* d_server;

    int d_xpos,d_ypos;
  };


#define X11Win ImageWindow_Autorefresh_X11

  /* Enhanced ImageWindow_X11-class that can accept an image and that watches
     X11 events to automatically redraw itself.
     To get fully automatic redrawing you have to create a new thread that
     calls RedrawForever() which will never return.
  */
  class ImageWindow_Autorefresh_X11 : public ImageWindow_X11
  {
  public:
    ImageWindow_Autorefresh_X11(bool useXv=false);
    ~ImageWindow_Autorefresh_X11();

    void Create(int w,int h,const char* title,X11Server* server=NULL,Window parent=0);
    void Close();

    void Display(const Image<Pixel>&);  // Input image contents may be destroyed


    // --- user interaction ---

    bool CheckForMouseMove(int& x,int& y);
    int  CheckForMouseButton(int& x,int& y);  /* Returns 1 for left, 2 for middle, 3 for right, 0 if no button was pressed.
						 Returns negative values for release events. */

    char CheckForKeypress();
    char WaitForKeypress();    // Image will be refreshed while waiting for the keypress.

    void CheckForRedraw();
    void RedrawForever();

  private:
    bool d_lastimg_was_RGB;
    bool d_lastimg_was_YUV;

    DisplayImage_X11* d_dispimg;      // the image to be displayed
    Image2RawRGB*     d_rgbtransform; // the transformation for image representation convertion

    void Redraw(XExposeEvent& ev);

    friend int MultiWindowRefresh(ImageWindow_Autorefresh_X11*const*,int nWindows);
  };

  /* MultiWindowRefresh also checks for keypresses. The window index of the window,
     in which the keypress has occured, is returned. Otherwise -1 is returned.
  */
  int MultiWindowRefresh(ImageWindow_Autorefresh_X11*const*,int nWindows);

  void DisplayX11(const Image<Pixel>&, int wait_usecs=0); // 0 -> wait for keypress
}

#endif