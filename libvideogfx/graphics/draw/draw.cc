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

#include <string.h>
#include <algorithm>
#include "libvideogfx/graphics/draw/draw.hh"
using namespace std;

namespace videogfx {

#define CLIP_TOP    1
#define CLIP_BOTTOM 2
#define CLIP_LEFT   4
#define CLIP_RIGHT  8

  static int OutCode(int x,int y,
		     int r_x0,int r_y0,int r_x1,int r_y1)    // clipping rectangle
  {
    int code=0;
    if (y>r_y1) code = CLIP_BOTTOM;
    else if (y<r_y0) code = CLIP_TOP;

    if (x>r_x1) code |= CLIP_RIGHT;
    else if (x<r_x0) code |= CLIP_LEFT;

    return code;
  }

  bool ClipLine(int& x0,int& y0,int& x1,int& y1,        // line parameters
		int r_x0,int r_y0,int r_x1,int r_y1)    // clipping rectangle
  {
    int outcode0 = OutCode(x0,y0, r_x0,r_y0, r_x1,r_y1);
    int outcode1 = OutCode(x1,y1, r_x0,r_y0, r_x1,r_y1);

    for (;;)
      {
	if (outcode0==0 && outcode1==0)
	  return true;

	if (outcode0 & outcode1)
	  return false;

	int clipcode;
	if (outcode0)
	  clipcode = outcode0;
	else
	  clipcode = outcode1;

	int x,y;
	if (clipcode & CLIP_TOP)
	  {
	    x = x0 + (x1-x0)*(r_y0-y0)/(y1-y0);
	    y = r_y0;
	  }
	else if (clipcode & CLIP_BOTTOM)
	  {
	    x = x0 + (x1-x0)*(r_y1-y0)/(y1-y0);
	    y = r_y1;
	  }
	else if (clipcode & CLIP_LEFT)
	  {
	    y = y0 + (y1-y0)*(r_x0-x0)/(x1-x0);
	    x = r_x0;
	  }
	else if (clipcode & CLIP_RIGHT)
	  {
	    y = y0 + (y1-y0)*(r_x1-x0)/(x1-x0);
	    x = r_x1;
	  }

	if (clipcode == outcode0)
	  { x0 = x; y0 = y; outcode0=OutCode(x0,y0, r_x0,r_y0,r_x1,r_y1); }
	else
	  { x1 = x; y1 = y; outcode1=OutCode(x1,y1, r_x0,r_y0,r_x1,r_y1); }
      }
  }


  void DrawTriangle(Image<Pixel>& img, const Point2D<double>* in_p, const Color<Pixel>* col)
  {
    ImageParam param = img.AskParam();
    Pixel c[3];
    Point2D<double> p[3];

    for (int i=0;i<4;i++)
      {
	BitmapChannel b = (BitmapChannel)i;
	if (!img.AskBitmap(b).IsEmpty())
	  {
	    for (int n=0;n<3;n++)
	      {
		c[n] = col[n].c[i];
		p[n].x = param.ChromaScaleH(b,in_p[n].x);
		p[n].y = param.ChromaScaleV(b,in_p[n].y);
	      }

	    DrawTriangle(img.AskBitmap(b), p, c);
	  }
      }
  }


  void ClearToBlack(Image<Pixel>& img)
  {
    Colorspace cs = img.AskParam().colorspace;

    if (cs == Colorspace_Greyscale)
      {
        Clear(img.AskBitmapY(),Pixel(0));
      }
    else if (cs == Colorspace_RGB)
      {
        Clear(img.AskBitmapR(),Pixel(0));
        Clear(img.AskBitmapG(),Pixel(0));
        Clear(img.AskBitmapB(),Pixel(0));
      }
    else if (cs == Colorspace_YUV)
      {
        Clear(img.AskBitmapY(),Pixel(16));
        Clear(img.AskBitmapU(),Pixel(128));
        Clear(img.AskBitmapV(),Pixel(128));
      }
    else if (cs == Colorspace_HSV)
      {
        Clear(img.AskBitmap(Bitmap_Hue       ),Pixel(0));
        Clear(img.AskBitmap(Bitmap_Saturation),Pixel(0));
        Clear(img.AskBitmap(Bitmap_Value     ),Pixel(0));
      }
  }


}
