/********************************************************************************
  $Header$

  purpose:

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
    24/Jan/2002 - Dirk Farin - Complete reimplementation based on old Image type.
    02/Jun/1999 - Dirk Farin - first implementation
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

#ifndef LIBVIDEOGFX_GRAPHICS_BASIC_IMAGE_HH
#define LIBVIDEOGFX_GRAPHICS_BASIC_IMAGE_HH

#include <assert.h>

#include <libvideogfx/types.hh>
#include <libvideogfx/graphics/datatypes/bitmap.hh>
#include <algorithm>

namespace videogfx {

  /** Image colorspace. */
  enum Colorspace
  {
    Colorspace_RGB,       ///< Red, green, blue.
    Colorspace_YUV,       ///< YUV; note that chroma planes may be subsampled to a lower resolution.
    Colorspace_Greyscale, ///< Luminance only.
    Colorspace_HSV,       ///< Hue, saturation, value.
    Colorspace_Invalid    ///< Undefined colorspace.
  };

  enum ChromaFormat
  {
    /** subsampling h:2 v:2 */ Chroma_420,
    /** subsampling h:2 v:1 */ Chroma_422,
    /** No subsampling      */ Chroma_444,
    Chroma_Invalid
  };

  enum BitmapChannel
  {
    Bitmap_Red = 0, Bitmap_Green = 1, Bitmap_Blue = 2,
    Bitmap_Y   = 0, Bitmap_Cb    = 1, Bitmap_Cr   = 2,
                    Bitmap_U     = 1, Bitmap_V    = 2,
    Bitmap_Hue = 0, Bitmap_Saturation = 1, Bitmap_Value = 2,
    Bitmap_Alpha=3
  };


  /** Check if chroma is horizontally subsampled. Usage of the more general #ChromaSubH()# is recommended. */
  inline bool IsSubH(ChromaFormat cf) { return cf != Chroma_444; }
  /** Check if chroma is vertically subsampled. Usage of the more general #ChromaSubV()# is recommended. */
  inline bool IsSubV(ChromaFormat cf) { return cf == Chroma_420; }
  /** Get horizontal subsampling factor. */
  inline int  ChromaSubH(ChromaFormat cf) { return (cf != Chroma_444) ? 2 : 1; }
  /** Get vertical subsampling factor. */
  inline int  ChromaSubV(ChromaFormat cf) { return (cf == Chroma_420) ? 2 : 1; }

  /** Image parameters. This structure contains all the information about image
      size, the colorspace, chroma subsampling, and alignments.
   */
  struct ImageParam
  {
    /** Create default parameters for an image. Size and colorspace is undefined,
	alignments and borders are off, no alpha and chroma is set to 4:4:4.
    */
    ImageParam() : width(0), height(0), halign(1), valign(1),
		   xoffset(0), yoffset(0),
		   colorspace(Colorspace_Invalid), has_alpha(false),
		   chroma(Chroma_444), reduced_chroma_resolution(true),
		   chroma_border(-1), chroma_halign(-1), chroma_valign(-1)
    {
      border=0;
    }

    ImageParam(int p_width,int p_height, Colorspace p_colorspace=Colorspace_YUV)
      : width(p_width), height(p_height), halign(1), valign(1),
	xoffset(0), yoffset(0),
	colorspace(p_colorspace), has_alpha(false),
	chroma(Chroma_420), reduced_chroma_resolution(true),
	chroma_border(-1), chroma_halign(-1), chroma_valign(-1)
    {
      border=0;
    }

    int width,height;  ///< image size (non-subsampled)
    int halign,valign; ///< alignments (non-subsampled)
    int border;        ///< border size (non-subsampled)
    int xoffset, yoffset;

    Colorspace colorspace; ///< colorspace used for image
    bool has_alpha;        ///< does the image have an alpha plane?

    ChromaFormat chroma;            ///< chroma subsampling used
    bool reduced_chroma_resolution; ///< use reduced memory for the chroma planes according to chroma format?
    int  chroma_border;       ///< border size for chroma (if sub-sampled)
    int  chroma_halign;       ///< horizontal alignment for chroma (if sub-sampled)
    int  chroma_valign;       ///< vertical alignment for chroma (if sub-sampled)

    /** Returns the width of chroma-planes (YUV only). For other colorspaces, the normal size is returned. */
    int  AskChromaWidth()  const
    {
      if (colorspace==Colorspace_YUV)
	return (width +ChromaSubH(chroma)-1)/ChromaSubH(chroma);
      else
	return width;
    }

    /** Returns the height of chroma-planes (YUV only). For other colorspaces, the normal size is returned. */
    int  AskChromaHeight() const
    {
      if (colorspace==Colorspace_YUV)
	return (height+ChromaSubV(chroma)-1)/ChromaSubV(chroma);
      else
	return height;
    }

    void AskChromaSizes(int& w,int &h) const;  ///< returns size of chroma planes
    int  AskChromaBorder() const; ///< returns size of chroma border
    int  AskChromaHAlign() const; ///< returns size of chroma horizontal alignment
    int  AskChromaVAlign() const; ///< returns size of chroma vertical alignment

    /// Width of the specified bitmap channel.
    int  BitmapWidth (BitmapChannel b) const { if (b==1||b==2) return AskChromaWidth();  else return width; }
    /// Height of the specified bitmap channel.
    int  BitmapHeight(BitmapChannel b) const { if (b==1||b==2) return AskChromaHeight(); else return height; }

    /// Scale the given horizontal coordinate by the sub-sampling factor of the specified bitmap channel.
    int  ChromaScaleH(BitmapChannel b,int x) const
    { if ((b==1||b==2) && colorspace==Colorspace_YUV) return x/ChromaSubH(chroma); else return x; }
    /// Scale the given vertical coordinate by the sub-sampling factor of the specified bitmap channel.
    int  ChromaScaleV(BitmapChannel b,int y) const
    { if ((b==1||b==2) && colorspace==Colorspace_YUV) return y/ChromaSubV(chroma); else return y; }

    /// Scale the given horizontal coordinate by the sub-sampling factor of the specified bitmap channel.
    double  ChromaScaleH(BitmapChannel b,double x) const
    { if ((b==1||b==2) && colorspace==Colorspace_YUV) return x/ChromaSubH(chroma); else return x; }
    /// Scale the given vertical coordinate by the sub-sampling factor of the specified bitmap channel.
    double  ChromaScaleV(BitmapChannel b,double y) const
    { if ((b==1||b==2) && colorspace==Colorspace_YUV) return y/ChromaSubV(chroma); else return y; }

    int nColorChannels() const
    {
      if (colorspace==Colorspace_Greyscale) return 1;
      else                                  return 3;
    }
  };



  /** The Image class combines up to four bitmaps to a colored image with optional alpha plane.
      The data is organized in independent color channels. The number of channels depends on the
      colorspace used. Greyscale images only use a single color channel, while a