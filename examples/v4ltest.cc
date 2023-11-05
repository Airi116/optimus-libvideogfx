
#include "../libvideogfx/graphics/fileio/v4l.hh"
#include "../libvideogfx/graphics/fileio/jpeg.hh"

#include <stdio.h>

#include <fstream>
#include <algorithm>
using namespace std;
using namespace videogfx;


int main(int argc,char** argv)
{
  V4L_Grabber grabber;
  grabber.SetDevice("/dev/video0");

  //grabber.SetGreyscaleMode();
  //grabber.DoChromaAvgOn422To420();
  // void SetResolution(int w,int h);

  grabber.StartGrabbing();

  Image<Pixel> img;

  char buf[100];
  for (int i=0;i<1;i++)
    {
      grabber.Grab(img);

      sprintf(buf,"grab%04d.jpg",i);
      WriteImage_JPEG(img,buf);
    }

  return 0;
}
