
/*********************************************************************
  libvideogfx/draw/draw.hh

  purpose:

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de
   - Alexander Staller, alexander.staller@engineer.com
   - Triangle rendering code based on the rasterization tutorial by
     Mihail Ivanchev, Hans Thoernquist

  modifications:
    30/Jan/2002 - Dirk Farin - New line clipping routine
    14/Aug/2000 - Alexander Staller - New functions: 
      DrawRectangle(), DrawCircle(), DrawEllipse(), DrawArrow()
      new class ArrowPainter, new implementation of DrawLine().
    14/Aug/2000 - Dirk Farin - new function: DrawFilledRectangleHV()
    17/Nov/1999 - Dirk Farin - converted bitmap drawing functions to
                               function templates.
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

#ifndef LIBVIDEOGFX_GRAPHICS_DRAW_DRAW_HH
#define LIBVIDEOGFX_GRAPHICS_DRAW_DRAW_HH

#include <math.h>
#include <algorithm>
#include <vector>
#include <stdio.h>

//#include <iostream>
//using namespace std;

#include <libvideogfx/graphics/datatypes/bitmap.hh>
#include <libvideogfx/graphics/datatypes/image.hh>
#include <libvideogfx/graphics/datatypes/primitives.hh>

#ifndef M_PI
#define M_PI 3.1415926535
#endif

namespace videogfx {

  /* This function-call is very inefficient but provides clipping. */
  template <class T> void DrawPoint     (Bitmap<T>&,int x,int y,T color);
  template <class T> void DrawPoint     (Image<T>& ,int x,int y,Color<T> color);

  /* Set all of the bitmap to the specified color. */
  template <class T> void Clear         (Bitmap<T>&,T color);
  template <class T> void Clear         (Image<T>& ,Color<T> color);
  /*              */ void ClearToBlack  (Image<Pixel>&);

  /* Draw H/V-aligned rectangle. */
  template <class T> void DrawRectangle(Bitmap<T>& bm,int x1,int y1,int w, int h,T color);
  template <class T> void DrawRectangle(Image<T>&  bm,int x1,int y1,int w, int h,Color<T> color);
  template <class T> void DrawRectangle_NEW(Bitmap<T>& bm,int x1,int y1,int x2,int y2,T color);
  template <class T> void DrawRectangle_NEW(Image<T>&  bm,int x1,int y1,int x2,int y2,Color<T> color);

  template <class T> void FillRectangle(Bitmap<T>&,int x0,int y0,int x1,int y1,T color);
  template <class T> void FillRectangle(Image<T>&, int x0,int y0,int x1,int y1,Color<T> color);

  /* Clip line to a rectangle. If false is returned, the line is not visible. */
  bool ClipLine(int& x0,int& y0,int& x1,int& y1,        // line parameters
		int r_x0,int r_y0,int r_x1,int r_y1);   // clipping rectangle

  // Fast line drawing, line-clipping is included.
  template <class T> void DrawLine(Bitmap<T>& bm,int x0,int y0,int x1, int y1,T color);
  template <class T> void DrawLine(Image<T>&  bm,int x0,int y0,int x1, int y1,Color<T> color);
  template <class T> void DrawLine(Bitmap<T>& bm,const Point2D<int>& p1,const Point2D<int>& p2,T color);
  template <class T> void DrawLine(Image<T>&  bm,const Point2D<int>& p1,const Point2D<int>& p2,Color<T> color);
  template <class T> void DrawLine(Bitmap<T>& bm,const Point2D<double>& p1,const Point2D<double>& p2,T color);
  template <class T> void DrawLine(Image<T>&  bm,const Point2D<double>& p1,const Point2D<double>& p2,Color<T> color);

  /* Only draws every nth dot. */
  template <class T> void DrawDottedLine(Bitmap<T>&,int x1,int y1,int x2,int y2,T color,int nth=4);

  // Draw a circle
  template <class T> void DrawCircle(Bitmap<T>& bm,int x0,int y0, int radius,T color,bool fill=false);
  template <class T> void DrawCircle(Image<T>&  bm,int x0,int y0, int radius,Color<T> color,bool fill=false);
  template <class T> void DrawCircle(Bitmap<T>& bm,const Point2D<int>& p, int radius,T color,bool fill=false);
  template <class T> void DrawCircle(Image<T>&  bm,const Point2D<int>& p, int radius,Color<T> color,bool fill=false);
  template <class T> void DrawCircle(Bitmap<T>& bm,const Point2D<double>& p, int radius,T color,bool fill=false);
  template <class T> void DrawCircle(Image<T>&  bm,const Point2D<double>& p, int radius,Color<T> color,bool fill=false);

  /* DrawTriangle(bitmap, pts[3], vertex_values[3] */
  template <class T> void DrawTriangle(Bitmap<T>& bm, const Point2D<double>* in_p, const T* c);
  void DrawTriangle(Image<Pixel>& img, const Point2D<double>* in_p, const Color<Pixel>* col);

  /* This function draws a line and places a head on one (x1,y1) if bothends==false or
     both (if bothends==true) sides of the line. */
  template <class T> void DrawArrow(Bitmap<T>& bm,int x0,int y0,int x1, int y1,T color,
				    double alpha=20.0,int len=7,bool bothends = false);
  template <class T> void DrawArrow(Image<T>& img,int x0,int y0,int x1, int y1,const Color<T>& color,
				    double alpha=20.0,int len=7,bool bothends = false);


  /* A PolylinePainter helps to draw polygons.
     You initialize the painter with a bitmap or an image and add points of the
     polygon to it. Finally, you have to stroke the path. You can close the path
     beforehand by calling "Close()".
  */
  template <class T> class PolylinePainter
  {
  public:
    PolylinePainter()
      : d_bm(NULL), d_img(NULL), d_began(false) { }

    void SetBitmap(Bitmap<T>& b, T value)   { d_bm = &b; d_img=NULL; d_color.c[0]=value; d_began=false; }
    void SetImage (Image<T>& b, Color<T> c) { d_img = &b, d_bm=NULL; d_color=c; d_began=false; }

    void AddPoint(int x,int y)
    {
      if (!d_began) { d_start.x = d_last.x = x; d_start.y = d_last.y = y; d_began=true; return; }
      if (d_bm)  DrawLine(*d_bm, d_last.x, d_last.y, x,y, d_color.c[0]);
      if (d_img) DrawLine(*d_img,d_last.x, d_last.y, x,y, d_color);
      d_last.x = x;
      d_last.y = y;
    }
    void AddPoint(Point2D<int> p) { AddPoint(p.x,p.y); }
    void Close() { AddPoint(d_start); Stroke(); }
    void Stroke() { d_began=false; }

  private:
    Bitmap<T>*   d_bm;
    Image<T>*    d_img;
    Color<T>     d_color;
    bool         d_began;
    Point2D<int> d_start;
    Point2D<int> d_last;
  };



  // utility

  template <class T>
  void drawPoints(Image<Pixel>& img, const std::vector<Point2D<T> >& pts, Color<Pixel> col)
  {
    for (int i=0;i<pts.size();i++)
      DrawPoint(img, pts[i].x, pts[i].y, col);
  }


  // ---------------------------- implementation -------------------------


  template <class T> void DrawPoint(Bitmap<T>& bm,int x,int y,T color)
  {
    if (x<-bm.AskXOffset() || y<-bm.AskYOffset()) return;
    if (x>=bm.AskWidth()-bm.AskXOffset() || y>=bm.AskHeight()-bm.AskYOffset()) return;

    bm.AskFrame()[y][x]=color;
  }

  template <class T> void DrawPoint(Image<T>& bm,int x,int y,Color<T> color)
  {
    ImageParam param = bm.AskParam();

    for (int i=0;i<4;i++)
      {
	BitmapChannel b = (BitmapChannel)i;
	if (!bm.AskBitmap(b).IsEmpty())
	  DrawPoint(bm.AskBitmap(b), param.ChromaScaleH(b,x), param.ChromaScaleV(b,y), color.c[i]);
      }
  }

  template <class T> void Clear(Bitmap<T>& bm,T color)
  {
    T*const* p = bm.AskFrame();
  
    if (sizeof(T)==1)
      {
	for (int y=-bm.AskYOffset();y<bm.AskHeight()-bm.AskYOffset();y++)
	  memset(&p[y][-bm.AskXOffset()],color,bm.AskWidth());
      }
    else
      {
	FillRectangle(bm,
		      -bm.AskXOffset(),-bm.AskYOffset(),
		      bm.AskWidth()-1-bm.AskXOffset(),bm.AskHeight()-1-bm.AskYOffset(),color);
      }
  }

  template <class T> void Clear         (Image<T>& img,Color<T> color)
  {
    ImageParam param = img.AskParam();

    for (int i=0;i<4;i++)
      {
	BitmapChannel b = (BitmapChannel)i;
	if (!img.AskBitmap(b).IsEmpty())
	  Clear(img.AskBitmap(b), color.c[i]);
      }
  }

  template <class T> void FillRectangle(Bitmap<T>& bm,int x0,int y0,int x1,int y1,T color)
  {
    T*const* p = bm.AskFrame();

    x0 = std::max(x0,-bm.AskXOffset());
    y0 = std::max(y0,-bm.AskYOffset());
    x0 = std::min(x0,-bm.AskXOffset()+bm.AskWidth()-1);
    y0 = std::min(y0,-bm.AskYOffset()+bm.AskHeight()-1);
    x1 = std::max(x1,-bm.AskXOffset());
    y1 = std::max(y1,-bm.AskYOffset());
    x1 = std::min(x1,-bm.AskXOffset()+bm.AskWidth()-1);
    y1 = std::min(y1,-bm.AskYOffset()+bm.AskHeight()-1);

    for (int y=y0;y<=y1;y++)
      for (int x=x0;x<=x1;x++)
	p[y][x] = color;
  }


  template <class T> void FillRectangle(Image<T>& bm, int x0,int y0,int x1,int y1,Color<T> color)
  {
    ImageParam param = bm.AskParam();

    for (int i=0;i<4;i++)
      {
	BitmapChannel b = (BitmapChannel)i;
	if (!bm.AskBitmap(b).IsEmpty())
	  FillRectangle(bm.AskBitmap(b),
			param.ChromaScaleH(b,x0), param.ChromaScaleV(b,y0),
			param.ChromaScaleH(b,x1), param.ChromaScaleV(b,y1),
			color.c[i]);
      }
  }


  // main function to draw a line very fast. Clipping is included, so don't think about it
  template <class T> void DrawLine(Bitmap<T>& bm,int x0,int y0,int x1, int y1,T color)
  {
    T*const* p = bm.AskFrame();

    if (!ClipLine(x0,y0,x1,y1,
		  -bm.AskXOffset(),-bm.AskYOffset(),
		  bm.AskWidth()-1-bm.AskXOffset(),bm.AskHeight()-1-bm.AskYOffset()))
      return;

    if (abs(y1-y0)>abs(x1-x0))
      {
	if (y1<y0) {  std::swap(x0,x1); std::swap(y0,y1); }

	int xinc;
	int dy = y1 - y0;
	int dx = x1 - x0;

	if (dx < 0)
	  {
	    xinc = -1;
	    dx = -dx;
	  }
	else
	  xinc = 1;

	int d = 2* dx - dy;
	int incrE = 2 * dx;  // Increment used for move to E
	int incrNE = 2 * (dx - dy);   // increment used for move to NE
	int y = y0;
	int x = x0;

	while (y <= y1)
	  {
	    p[y][x] = color;
     
	    if (d <= 0)   // Choose E
	      {
		d = d + incrE;
		y++;
	      }
	    else         // Choose NE
	      {
		d = d + incrNE;
		y++;
		x = x + xinc;
	      }
	  }
      }
    else
      {