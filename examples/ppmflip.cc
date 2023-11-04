

#include "../libvideogfx/graphics/fileio/ppm.hh"

#include <fstream>
#include <algorithm>
using namespace std;
using namespace videogfx;


int main(int argc,char** argv)
{
  ifstream istr(argv[1]);
  ofstream ostr(argv[2]);

  Image<Pixel> img;
  ReadImage_PPM(img,istr);