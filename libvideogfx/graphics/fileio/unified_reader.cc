
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
#include "libvideogfx/graphics/fileio/unified_reader.hh"
//#include "libvideogfx/graphics/fileio/mplayer.hh"
#ifdef HAVE_FFMPEG
#include "libvideogfx/graphics/fileio/ffmpeg.hh"
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

#ifndef strncasecmp
inline int strncasecmp(const char* a, const char* b, int len)
{
  for (int i=0;i<len;i++)
    {
      if (tolower(a[i]) != tolower(b[i])) return false;
      if (a[i]==0) return true;
    }

  return true;
}
#endif

#ifndef getline
ssize_t getline(char** lineptr, size_t* n, FILE* stream)
{
  if (*lineptr==0)
    {
      *lineptr = (char*)malloc(1000);
      *n = 1000;
    }

  int i=0;
  for (;;)
    {
      int c = fgetc(stream);
      if (c==EOF) break;
      assert(i+1 < *n);   // TODO: realloc line-buffer
      (*lineptr)[i] = c;
      if (c=='\n') break;
      i++;
    }

  assert(i+1 < *n);   // TODO: realloc line-buffer
  (*lineptr)[i]=0;

  return i;
}
#endif

namespace videogfx {
  using namespace std;

  const ReaderStageFactory* UnifiedImageReader::s_plugins[MAX_READER_PLUGINS];
  int UnifiedImageReader::s_nplugins;

  static const char* configuration_file = "%s/.libvideogfxrc";


  static char* ExtractNextMacroOption(const char* spec,char& nextc);
  static void RemoveMacroOption(char* spec);

  char* ExpandMacros(char* spec)
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
#define MAX_RCPATH_LEN 500
	    char buf[MAX_RCPATH_LEN];
	    snprintf(buf,MAX_RCPATH_LEN,configuration_file,getenv("HOME"));

	    FILE* fh = fopen(buf,"r");
	    if (!fh)
	      {
		assert(false); // "no macro definition file found in user home directory");