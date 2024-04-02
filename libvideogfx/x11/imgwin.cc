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

#include "config.h"

#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <iomanip>
using namespace std;

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <X11/Xutil.h>

#if HAVE_XV
#include <X11/extensions/Xvlib.h>
#endif


#include "server.hh"
#include "imgwin.hh"

namespace videogfx {

  struct X11SpecificData
  {
    Display*  d_display;
    Window    d_win;
  };


  ImageWindow_X11::ImageWindow_X11()
    : d_initialized(false),
      d_server(NULL),
      d_xpos(-1),d_ypos(-1)
  {
    d_x11data = new X11SpecificData;
  }


  ImageWindow_X11::~ImageWindow_X11()
  {
    //printf("ImageWindow_X11::~ImageWindow_X11()\n");
    Close();

    if (d_x11data) { delete d_x11data; d_x11data=NULL; }
    if (d_server) delete d_server;
  }

  void ImageWindow_X11::Close()
  {
    if (!d_initialized)
      return;

    XUnmapWindow(d_x11data->d_display , d_x11data->d_win);
    XFlush(d_x11data->d_display);
    d_initialized=false;
  }


  Window   ImageWindow_X11::AskWindow()  { assert(d_initialized); return d_x11data->d_win; }
  Display* ImageWindow_X11::AskDisplay() { return d_x11data->d_display; }


  void ImageWindow_X11::Create(int w,int h,const char* title,X11Server* server,Window parent)
  {
    assert(!d_initialized);

    // Get X11 server.

    if (d_server) delete d_server;

    if (server)
      d_server = new X11ServerConnection(server);
    else
      d_server = new X11ServerConnection;

    d_x11data->d_display = d_server->AskDisplay();

    int screen = DefaultScreen(d_x11data->d_display);
    Window rootwin = RootWindow(d_x11data->d_display,screen);


    // Choose VisualInfo

    XVisualInfo vinfo;
    //bool use_cmap8=false;

    if (XMatchVisualInfo(d_x11data->d_display, screen, 16, TrueColor, &vinfo))
      {
      }
    else
      if (XMatchVisualInfo(d_x11data->d_display, screen, 15, TrueColor, &vinfo))
	{
	}
      else
	if (XMatchVisualInfo(d_x11data->d_display, screen, 24, TrueColor, &vinfo))
	  {
	  }
	else
	  if (XMatchVisualInfo(d_x11data->d_display, screen, 32, TrueColor, &vinfo))
	    {
	    }
	  else
	    if (XMatchVisualInfo(d_x11data->d_display, screen,  8, PseudoColor, &vinfo))
	      {
		// use_cmap8=true;
	      }
	    else
	      if (XMatchVisualInfo(d_x11data->d_display, screen,  4, StaticGray, &vinfo))
		{
		}
#if 0
	      else
		if (XMatchVisualInfo(d_x11data->d_display, screen,  8, GrayScale, &vinfo))
		  {
		  }
#endif
		