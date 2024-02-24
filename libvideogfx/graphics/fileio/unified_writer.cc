
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

#include <libvideogfx/graphics/fileio/unified_writer.hh>
#include <libvideogfx/graphics/fileio/jpeg.hh>
#include <libvideogfx/graphics/fileio/ppm.hh>
#include <libvideogfx/graphics/fileio/yuv.hh>
#include <libvideogfx/graphics/fileio/png.hh>
#include <libvideogfx/graphics/fileio/uyvy.hh>
#include <libvideogfx/graphics/color/colorspace.hh>
#include <libvideogfx/graphics/visualize/regions.hh>
#include "libvideogfx/graphics/datatypes/primitives.hh"
#include "libvideogfx/graphics/draw/scale.hh"
#include "libvideogfx/graphics/draw/pixelops.hh"
#include "config.h"

#if HAVE_X11
#include <libvideogfx/x11/imgwin.hh>
#endif
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>

namespace videogfx {
  using namespace std;

  const WriterStageFactory* UnifiedImageWriter::s_plugins[MAX_WRITER_PLUGINS];
  int UnifiedImageWriter::s_nplugins;


  void WriterStage::AppendAtEnd(WriterStage* next_stage)
  {
    if (next) next->AppendAtEnd(next_stage);
    else next = next_stage;
  }

  void UnifiedImageWriter::RegisterPlugin(const WriterStageFactory* fact)
  {
    assert(s_nplugins < MAX_WRITER_PLUGINS);
    s_plugins[s_nplugins++] = fact;
  }

  const char* UnifiedImageWriter::AskPluginName(int idx)
  {
    if (idx >= s_nplugins)
      return NULL;
    else
      return s_plugins[idx]->AskName();
  }

  bool UnifiedImageWriter::SetOutput(const char* output_specification)
  {
    char* speccopy = new char[strlen(output_specification)+1];
    strcpy(speccopy,output_specification);

    char* spec = ExpandMacros(speccopy);
    delete[] speccopy;

    if (d_writer_pipeline) delete d_writer_pipeline;
    d_writer_pipeline = NULL;

    while (*spec)
      {
	bool found_one = false;
	for (int i=0;i<s_nplugins;i++)
	  {
	    WriterStage* newpipe = s_plugins[i]->ParseSpec(&spec);
	    if (newpipe)
	      {
		if (d_writer_pipeline==NULL)
		  d_writer_pipeline = newpipe;
		else
		  d_writer_pipeline->AppendAtEnd(newpipe);

		found_one = true;
		break;
	      }
	  }

	if (!found_one)
	  {
	    if (d_writer_pipeline) delete d_writer_pipeline;
	    d_writer_pipeline=NULL;
	    delete[] spec;
	    return false;
	  }
      }

    delete[] spec;
    return true;
  }


  void UnifiedImageWriter::WriteImage(const Image<Pixel>& img)
  {
    assert(d_writer_pipeline); // "no writer specified");
    d_writer_pipeline->WriteImage(img);
  }

  WriterStageFactory::WriterStageFactory() { UnifiedImageWriter::RegisterPlugin(this); }



  // ------------------------------------------------------------------------------

#define MAX_FILENAME_LEN 1000

  class WriterStage_SglPictures : public WriterStage
  {
  public:
    WriterStage_SglPictures() : d_next(0) { d_filename_template[0]=0; }

    enum FileFormat { Format_Undefined, Format_JPEG, Format_PNG, Format_PPM, Format_UYVY };

    void SetFilenameTemplate(const char* t) { assert(strlen(t)<MAX_FILENAME_LEN); strcpy(d_filename_template,t); }
    void SetFormat(WriterStage_SglPictures::FileFormat f) { d_format=f; }

    void WriteImage(const Image<Pixel>& img)
    {
      GenerateFilename();

      Image<Pixel> conv_img;
      

      switch (d_format)
	{
	case WriterStage_SglPictures::Format_JPEG:
	  assert(JPEG_Supported()); //"JPEG support not compiled in");
	  conv_img = ChangeColorspace_NoCopy(img, Colorspace_YUV, Chroma_420);
	  WriteImage_JPEG(d_filename,conv_img);
	  break;

	case WriterStage_SglPictures::Format_PNG:
	  assert(PNG_Supported()); //"PNG support not compiled in");
	  conv_img = ChangeColorspace_NoCopy(img, Colorspace_RGB);
	  WriteImage_PNG (d_filename,conv_img);
	  break;

	case WriterStage_SglPictures::Format_PPM:
	  conv_img = ChangeColorspace_NoCopy(img, Colorspace_RGB);
	  WriteImage_PPM (d_filename,conv_img);
	  break;

	case WriterStage_SglPictures::Format_UYVY:
	  {
	    conv_img = ChangeColorspace_NoCopy(img, Colorspace_YUV, Chroma_422);

	    // TODO
	    assert(conv_img.AskWidth()==704);
	    assert(conv_img.AskHeight()==568);

	    ofstream ostr(d_filename);
	    WriteImage_UYVY (ostr, conv_img);
	  }
	  break;
	}

      d_next++;

      if (next) next->WriteImage(img);
    }

  private:
    int d_next;
    FileFormat d_format;

    char d_filename[MAX_FILENAME_LEN];
    char d_filename_template[MAX_FILENAME_LEN];

    void GenerateFilename() const
    {
      assert(strlen(d_filename_template)<MAX_FILENAME_LEN);
      snprintf((char*)d_filename,MAX_FILENAME_LEN,d_filename_template,d_next);
    }
  };

  static class WriterStageFactory_SglPictures : public WriterStageFactory
  {
  public:
    WriterStage* ParseSpec(char** spec) const
    {
      WriterStage_SglPictures::FileFormat f = WriterStage_SglPictures::Format_Undefined;

      if (CheckSuffix(*spec, "jpeg") || CheckSuffix(*spec, "jpg"))
	f = WriterStage_SglPictures::Format_JPEG;
      else if (CheckSuffix(*spec, "pgm") || CheckSuffix(*spec, "ppm"))
	f = WriterStage_SglPictures::Format_PPM;
      else if (CheckSuffix(*spec, "png"))
	f = WriterStage_SglPictures::Format_PNG;
      else if (CheckSuffix(*spec, "uyvy"))
	f = WriterStage_SglPictures::Format_UYVY;

      if (f != WriterStage_SglPictures::Format_Undefined)
	{
	  WriterStage_SglPictures* pl = new WriterStage_SglPictures;
	  char* name = ExtractNextOption(*spec);
	  pl->SetFilenameTemplate(name);
	  pl->SetFormat(f);
	  delete[] name;
	  RemoveOption(*spec);

	  return pl;
	}
      else
	return NULL;
    }

    const char* Name() const { return "writer: picture sequence"; }

  } singleton_sglpictures;


  // ------------------------------------------------------------------------------

  class WriterStage_YUVStream : public WriterStage
  {
  public:
    WriterStage_YUVStream() { }

    void SetFilename(const char* t)
    {
      assert(strlen(t)<MAX_FILENAME_LEN);
      strcpy(d_filename,t);
      // d_ostr.close();
      /* NOTE: strange C++ behaviour: if the stream is closed,
	 and opened thereafter, nothing is written into the file... */
      d_ostr.open(d_filename, std::ios::out | std::ios::binary);
      d_writer.SetYUVStream(d_ostr);
    }

    void SetAlphaFilename(const char* t)
    {
      assert(strlen(t)<MAX_FILENAME_LEN);
      strcpy(d_alpha_filename,t);
      d_alpha_ostr.open(d_alpha_filename, std::ios::out | std::ios::binary);
      d_writer.SetAlphaStream(d_alpha_ostr);
    }

    void SetInterleavedChroma(bool flag) { d_writer.SetWriteInterleaved(flag); }

    void WriteImage(const Image<Pixel>& img)
    {
      Image<Pixel> conv_img;

      if (img.AskParam().colorspace != Colorspace_YUV)
	conv_img = ChangeColorspace_NoCopy(img, Colorspace_YUV, Chroma_420);
      else
	conv_img = img;

      ImageParam param = img.AskParam();
      d_writer.WriteImage(conv_img);

      if (next) next->WriteImage(img);
    }

  private:
    char d_filename[MAX_FILENAME_LEN];
    char d_alpha_filename[MAX_FILENAME_LEN];
    ofstream d_ostr;
    ofstream d_alpha_ostr;
    ImageWriter_YUV1 d_writer;
  };

  static class WriterStageFactory_YUVStream : public WriterStageFactory
  {
  public:
    WriterStage* ParseSpec(char** spec) const
    {
      if (CheckSuffix(*spec, "yuv"))
	{
	  WriterStage_YUVStream* wr = new WriterStage_YUVStream;
	  char* name = ExtractNextOption(*spec);
	  wr->SetFilename(name);
	  delete[] name;
	  RemoveOption(*spec);

	  wr->SetInterleavedChroma(false);

	  for (;;)
	    {
	      if (MatchOption(*spec, "interleaved"))
		{
		  RemoveOption(*spec);
		  wr->SetInterleavedChroma(true);
		}
	      else if (MatchOption(*spec, "alpha"))
		{
		  RemoveOption(*spec);
		  char* alphaname = ExtractNextOption(*spec);
		  wr->SetAlphaFilename(alphaname);
		  delete[] alphaname;
		  RemoveOption(*spec);
		}
	      else
		break;
	    }

	  return wr;
	}
      else
	return NULL;
    }

    const char* Name() const { return "writer: raw YUV stream"; }

  } singleton_yuvstream;

  // ------------------------------------------------------------------------------

#if HAVE_X11
  class WriterStage_X11Output : public WriterStage
  {
  public:
    WriterStage_X11Output() { d_width=d_height=-1; SetWindowName("output"); }

    void SetWindowName(const char* name)
    {
      assert(strlen(name)<MAX_FILENAME_LEN);
      strcpy(d_winname,name);
    }

    void WriteImage(const Image<Pixel>& img)
    {
      if (img.AskWidth() != d_width || img.AskHeight() != d_height)
	{
	  d_win.Close();
	  d_width  = img.AskWidth();
	  d_height = img.AskHeight();
	  d_win.Create(d_width, d_height, d_winname);
	}

      Image<Pixel> conv_img;

      d_win.Display(img);

      if (next) next->WriteImage(img);
    }

  private:
    char d_winname[MAX_FILENAME_LEN];
    ImageWindow_Autorefresh_X11 d_win;

    int d_width, d_height;
  };

  static class WriterStageFactory_X11Output : public WriterStageFactory
  {
  public:
    WriterStage* ParseSpec(char** spec) const
    {
      if (MatchOption(*spec, "win"))
	{
	  WriterStage_X11Output* wr = new WriterStage_X11Output;
	  RemoveOption(*spec);

	  return wr;
	}
      else
	return NULL;
    }

    const char* Name() const { return "writer: X11 window"; }

  } singleton_x11output;
#endif


  // ------------------------------------------------------------------------------

  class WriterStage_AlphaToGrey : public WriterStage
  {
  public:
    WriterStage_AlphaToGrey() { }

    void WriteImage(const Image<Pixel>& img)
    {
      Image<Pixel> newimg;
      newimg.Create(img.AskWidth(), img.AskHeight(), Colorspace_Greyscale);
      newimg.ReplaceBitmap(Bitmap_Y, img.AskBitmapA());

      assert(next);
      next->WriteImage(newimg);
    }
  };

  static class WriterStageFactory_AlphaToGrey : public WriterStageFactory
  {
  public:
    WriterStage* ParseSpec(char** spec) const
    {
      if (MatchOption(*spec, "alpha2grey"))
	{
	  WriterStage_AlphaToGrey* wr = new WriterStage_AlphaToGrey;
	  RemoveOption(*spec);

	  return wr;
	}
      else
	return NULL;
    }

    const char* Name() const { return "filter: alpha to greyscale"; }

  } singleton_alphatogrey;

  // ------------------------------------------------------------------------------

  class WriterStage_AlphaOverlay : public WriterStage
  {
  public:
    WriterStage_AlphaOverlay() { d_col=Color<Pixel>(255,0,0); }

    void SetColor(int r,int g,int b)
    {
      d_col=Color<Pixel>(r,g,b);
    }

    void WriteImage(const Image<Pixel>& img)
    {
      Image<Pixel> newimg;
      ChangeColorspace(newimg, img, Colorspace_RGB);
      OverlayAlphaMask(newimg.AskBitmapR(), img.AskBitmapA(), d_col.c[0]);
      OverlayAlphaMask(newimg.AskBitmapG(), img.AskBitmapA(), d_col.c[1]);
      OverlayAlphaMask(newimg.AskBitmapB(), img.AskBitmapA(), d_col.c[2]);

      assert(next);
      next->WriteImage(newimg);
    }

  private:
    Color<Pixel> d_col;
  };

  static class WriterStageFactory_Overlay : public WriterStageFactory
  {
  public:
    WriterStage* ParseSpec(char** spec) const
    {
      if (MatchOption(*spec, "alphaoverlay"))
	{
	  WriterStage_AlphaOverlay* wr = new WriterStage_AlphaOverlay;
	  RemoveOption(*spec);

	  if (MatchOption(*spec, "color"))
	    {
	      RemoveOption(*spec);
	      int r = ExtractNextNumber(*spec); RemoveOption(*spec);
	      int g = ExtractNextNumber(*spec); RemoveOption(*spec);
	      int b = ExtractNextNumber(*spec); RemoveOption(*spec);

	      wr->SetColor(r,g,b);