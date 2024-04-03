/*********************************************************************
  libvideogfx/x11/server.hh

  purpose:
    Get access to an X11 server. Create a object of type
    X11ServerConnection and keep it as long as you need access to
    the X11 server.
    Currently, only use the default constructor, which gives you
    a connection to the default X-server.

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
 ********************************************************************************
    LibVideoGfx - video processing library
    Copyright (C) 2002  Dirk Farin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR