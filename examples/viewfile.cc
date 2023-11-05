// use option -xv to open image with Xv extension

#include "../libvideogfx/x11/imgwin.hh"
#include "../libvideogfx/graphics/fileio/unified_reader.hh"

#include <unistd.h>
#include <iostream>
using namespace videogfx;
using namespace std;

int main(int argc,char** argv)
{
  ImageWindow_Autorefresh_X11 win;
  Image<Pixel> img;

  UnifiedImageLoader reader;
  reader.SetInput(argv[1]);

  reader.ReadImage(img);

  win.Create(img.AskWidth(),img.AskHeight(),"test");
  win.Display(img)