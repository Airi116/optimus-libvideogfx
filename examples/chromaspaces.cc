// use option -xv to open image with Xv extension

#include "../libvideogfx/x11/imgwin.hh"
#include "../libvideogfx/graphics/color/colorspace.hh"
#include "../libvideogfx/graphics/draw/draw.hh"
#include <unistd.h>
using namespace videogfx;


void FillGradient(Bitmap<Pixel>& bmx,Bitmap<Pixel>& bmy)
{
  Pixel*const* px = bmx.AskFrame();
  Pixel*const* py = bmy.AskFrame();

  for (int y=0;y<256;y++)
    for (int x=0;x<256;x++)
      {
	px[y][x] = x;
	py[y][x] = y;
      }
}


int main(int argc,char** argv)
{
  ImageWindow_Autorefresh_X11 win_rgb;
  ImageWindow_Autorefresh_X11 win_yuv;
  ImageWindow_Autorefresh_X11 win_hsv;

  ImageParam spec;
  spec.width   = 256;