

#if 0

/********************************************************************************
    LibVideoGfx - video processing library
    Copyright (C) 2004  Dirk Farin

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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "config.h"
#include "libvideogfx/graphics/fileio/unified_loader.hh"
#if LINUX
#  include "libvideogfx/graphics/fileio/mplayer.hh"
#endif
#include "libvideogfx/graphics/fileio/png.hh"
#include "libvideogfx/graphics/fileio/ppm.hh"
#include "libvideogfx/graphics/fileio/uyvy.hh"
#include "libvideogfx/graphics/fileio/yuv.hh"
#include "libvideogfx/graphics/fileio/jpeg.hh"
#include "libvideogfx/graphics/color/colorspace.hh"
#include "libvideogfx/graphics/draw/draw.hh"
#include "libvideogfx/graphics/draw/scale.hh"
#include "libvideogfx/graphics/draw/format.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>

namespace videogfx {
  using namespace std;

  const FileIOFactory* UnifiedImageLoader::s_plugins[MAX_LOADER_PLUGINS];
  int UnifiedImageLoader::s_nplugins;


  static char* ExtractNextMacroOption(const char* spec,char& nextc);
  static void RemoveMacroOption(char* spec);

  static char* ExpandMacros(char* spec)
  {
    //cout << "----------- start (" << spec << ") -------------\n";

    char* result = NULL;

    while (*spec)
      {
	char  nextc;
	char* option = ExtractNextMacroOption(spec, nextc);
	RemoveMacroOption(spec);

	//cerr << "macro-option: " << option << endl;
	//cerr << "remains: " << spec << endl;
	//cerr << "delimiter: " << (nextc ? nextc : '0') << endl;

	if (option[0]=='=') // is a macro
	  {
	    char buf[1000];
	    sprintf(buf,"%s/.libvideogfxrc",getenv("HOME"));

	    FILE* fh = fopen(buf,"r");
	    if (!fh)
	      {
		assert(false); // "no macro definition file found in user home directory");
		return NULL;
	      }

	    char* macro = option;
	    char* line=NULL;
	    size_t n=0;

	    for(;;)
	      {
		getline(&line,&n,fh);
		line[strlen(line)-1]=0;
		if (feof(fh))
		  break;

		if (strncmp(macro,line,strlen(macro))==0)
		  {
		    char* replacement;

		    option = new char[strlen(line)-strlen(macro)];
		    strcpy(option,line+strlen(macro)+1);
		    break;
		  }
	      }

	    if (line) free(line);
	    delete[] macro;

	    fclose(fh);


	    // recursive expansion

	    char* recurs = ExpandMacros(option);
	    delete[] option;
	    option = recurs;
	  }


	// Last or expanded option is now in "option". Append to "result"

	//cout << "COMBINE ->\n";

	//cout << "result |" << (result ? result : "NULL") << endl;
	//cout << "option |" << option << endl;
	//cout << "spec   |" << spec << endl;
	//cout << "nextc  |" << nextc << endl;

	if (!result)
	  {
	    result = new char[1+strlen(option)+1];
	    if (*spec) { sprintf(result,"%s%c",option,nextc); }
	    else sprintf(result,"%s",option);
	    delete[] option;
	  }
	else
	  {
	    char* newresult = new char[strlen(result)+1+strlen(option)+1];
	    if (*spec) sprintf(newresult,"%s%s%c",result,option,nextc);
	    else sprintf(newresult,"%s%s",result,option);
	    delete[] option;
	    delete[] result;
	    result = newresult;
	  }

	//cout << ">>>>>> |" << (result ? result : "NULL") << endl;
	//cout << "END COMBINE\n";
	//cout << "result: " << result << endl;
      }

    //cout << "----------- quit (" << result << ") -------------\n";
    return result;
  }



  void LoaderPlugin::SetPrevious(LoaderPlugin* previous)
  {
    if (prev) prev->SetPrevious(previous);
    else prev = previous;
  }

  void UnifiedImageLoader::RegisterPlugin(const FileIOFactory* fact)
  {
    assert(s_nplugins < MAX_LOADER_PLUGINS);
    s_plugins[s_nplugins++] = fact;
  }


  bool UnifiedImageLoader::SetInput(const char* input_specification)
  {
    char* speccopy = new char[strlen(input_specification)+1];
    strcpy(speccopy,input_specification);

    char* spec = ExpandMacros(speccopy);
    delete[] speccopy;

    if (d_loader_pipeline) delete d_loader_pipeline;
    d_loader_pipeline = NULL;
    d_framenr=0; // reading is starting from the beginning


    while (*spec)
      {
	bool found_one = false;
	for (int i=0;i<s_nplugins;i++)
	  {
	    LoaderPlugin* newpipe = s_plugins[i]->ParseSpec(&spec);
	    if (newpipe)
	      {
		newpipe->SetPrevious(d_loader_pipeline);
		d_loader_pipeline=newpipe;
		found_one = true;
		break;
	      }
	  }

	if (!found_one)
	  {
	    delete d_loader_pipeline;
	    d_loader_pipeline=NULL;
	    delete[] spec;
	    return false;
	  }
      }

    delete[] spec;
    return true;
  }


  int  UnifiedImageLoader::AskNFrames() const { assert(d_loader_pipeline); return d_loader_pipeline->AskNFrames(); }
  bool UnifiedImageLoader::IsEOF() const { assert(d_loader_pipeline); return d_loader_pipeline->IsEOF(); }


  bool UnifiedImageLoader::SkipToImage(int nr)
  {
    if (d_framenr==nr)
      return true;

    assert(d_loader_pipeline);
    d_framenr = nr;
    d_preload.Release();
    return d_loader_pipeline->SkipToImage(nr);
  }


  void UnifiedImageLoader::ReadImage(Image<Pixel>& img)
  {
    assert(d_loader_pipeline); // "no loader specified");

    d_framenr++;

    if (!d_preload.IsEmpty())
      {
	img = d_preload;
	d_preload.Release();
	return;
      }

    d_loader_pipeline->ReadImage(img);

    if (d_colorspace != Colorspace_Invalid || d_chroma != Chroma_Invalid)
      {
	Colorspace   colorspace = (d_colorspace == Colorspace_Invalid ? img.AskParam().colorspace : d_colorspace);
	ChromaFormat chroma     = (d_chroma     == Chroma_Invalid     ? img.AskParam().chroma     : d_chroma);

	Image<Pixel> tmp;
	ChangeColorspace(tmp,img, colorspace, chroma);
	img = tmp;
      }
  }


  void UnifiedImageLoader::PeekImage(Image<Pixel>& img)
  {
    if (d_preload.IsEmpty())
      ReadImage(d_preload);

    img = d_preload;
  }


  int  UnifiedImageLoader::AskWidth() const
  {
    if (width) return width;

    UnifiedImageLoader* ncthis = const_cast<UnifiedImageLoader*>(this);

    if (d_preload.IsEmpty())
      ncthis->ReadImage(ncthis->d_preload);

    ncthis->width = d_preload.AskWidth();
    return width;
  }


  int  UnifiedImageLoader::AskHeight() const
  {
    if (height) return height;

    UnifiedImageLoader* ncthis = const_cast<UnifiedImageLoader*>(this);

    if (d_preload.IsEmpty())
      ncthis->ReadImage(ncthis->d_preload);

    ncthis->height = d_preload.AskHeight();
    return height;
  }



  FileIOFactory::FileIOFactory() { UnifiedImageLoader::RegisterPlugin(this); }


  char* ExtractNextOption(const char* spec)
  {
    const char* p = index(spec,':');
    if (!p) p=spec+strlen(spec);
    int len = (p-spec);

    char* opt = new char[len+1];
    strncpy(opt,spec,len);
    opt[len]=0;

    return opt;
  }

  static char* ExtractNextMacroOption(const char* spec,char& nextc)
  {
    if (*spec != '=')
      { nextc=':'; return ExtractNextOption(spec); }

    const char* p = spec;

    while (*p && *p!='/' && *p != ':') p++;
    nextc = *p;
    if (!*p) p=0;

    if (!p) p=spec+strlen(spec);
    int len = (p-spec);

    char* opt = new char[len+1];
    strncpy(opt,spec,len);
    opt[len]=0;

    return opt;
  }

  int  ExtractNextNumber(const char* spec)
  {
    char* o = ExtractNextOption(spec);
    int num = atoi(o);
    delete[] o;
    return num;
  }

  bool MatchOption(const char* spec,const char* option)
  {
    const char* p = index(spec,':');
    if (!p) p=spec+strlen(spec);
    int len = (p-spec);

    if (strlen(option) != len) return false;
    return strncasecmp(option,spec,len)==0;
  }

  bool CheckSuffix(const char* spec,const char* suffix)
  {
    const char* p = index(spec,':');
    if (!p) p=spec+strlen(spec);
    int len = (p-spec);

    if (len<strlen(suffix)+2)
      return false;

    p -= strlen(suffix)+1;

    if (*p != '.') return false;
    p++;

    return strncasecmp(suffix,p,strlen(suffix))==0;
  }

  void RemoveOption(char* spec)
  {
    const char* p = index(spec,':');
    if (!p) { *spec=0; return; }

    int len = (p-spec);

    memmove(spec,spec+len+1,strlen(spec)-len+1);
  }

  static void RemoveMacroOption(char* spec)
  {
    if (*spec!='=')
      { RemoveOption(spec); return; }

    while (*spec != '/' && *spec && *spec !=':')
      memmove(spec,spec+1,strlen(spec));

    if (*spec == '/' || *spec == ':')
      memmove(spec,spec+1,strlen(spec));
  }

  bool ExtractSize(char* spec,int& w,int& h)
  {
    char* s = ExtractNextOption(spec);
    char* i = index(spec,'x');
    if (!i) { delete[] s; w=h=0; return false; }

    // check size format (must be '1234x1234' or '1234X1234')
    {
      char* n;
      n=s;
      do { if (!isdigit(*n)) { delete[] s; w=h=0; return false; } n++; } while(*n && (*n!='x' && *n!='X'));
      if (*n!='x' && *n!='X') { delete[] s; w=h=0; return false; }
      n++;
      do { if (!isdigit(*n)) { delete[] s; w=h=0; return false; } n++; } while(*n);
    }

    w = atoi(s);
    h = atoi(i+1);

    delete[] s;

    RemoveOption(spec);
    return true;
  }

  void ExtractSize(char* spec,int& w,int& h,int default_w,int default_h)
  {
    if (!ExtractSize(spec,w,h))
      {
	w = default_w;
	h = default_h;
      }
  }



  // ------------------------------------------------------------------------------


  class LoaderPlugin_RGB : public LoaderPlugin
  {
  public:
    LoaderPlugin_RGB() { id=0; }

    int  AskNFrames() const { return 9999999; }
    bool IsEOF() const { return false; }

    bool SkipToImage(int nr) { id = (nr%3); }
    void ReadImage(Image<Pixel>& img)
    {
      ImageParam spec(d_w,d_h);
      img.Create(spec);