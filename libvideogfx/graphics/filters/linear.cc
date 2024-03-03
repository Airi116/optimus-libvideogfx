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

#include "libvideogfx/graphics/filters/linear.hh"

#include <math.h>
#include <iostream>
using namespace std;

namespace videogfx {

#if 0
  /* Low-pass filtering is done using this kernel:

     1   /  0  1  0  \
     - * |  1  4  1  |
     8   \  0  1  0  /
  */
  void LowPass_5pt(const Image_YUV<Pixel>& img,Image_YUV<Pixel>& dest);


  /* Low-pass filtering is done using this kernel:

     1   /  1  1  1  \
     - * |  1  1  1  |
     9   \  1  1  1  /
  */
  void LowPass_3x3mean(const Image_YUV<Pixel>& img,Image_YUV<Pixel>& dest);

  /* Apply interpolation filter with the kernel (1 3 3 1)//4 .
   * 'src' and 'dst' may be the same.
   */
  void InterpolateH_Tap4(const Pixel*const* src,Pixel*const* dst,int dst_width,int height);
  void InterpolateV_Tap4(const Pixel*const* src,Pixel*const* dst,int width,int dst_height);
#endif


#if 0
  void LowPass_5pt(const Image_YUV<Pixel>& img,Image_YUV<Pixel>& dest)
  {
    // Get image parameters and assure that they are in the right format.
    ImageParam_YUV param;
    img.GetParam(param);

    ImageParam_YUV param2;
    dest.GetParam(param2);

    assert(param.chroma ==Chroma444);
    assert(param2.chroma==Chroma44