#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ffmpeg_writer.hh"

#include <libvideogfx/graphics/color/colorspace.hh>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>

#ifndef UINT64_C
#define UINT64_C(x) (x ## ULL)
#endif

#ifndef INT64_C
#define INT64_C(x) (x ## LL)
#endif

extern "C" {
#ifdef HAVE_FFMPEG_AVUTIL_H
#include <ffmpeg/avutil.h>
#include <ffmpeg/mathematics.h>
#else
#include <libavutil/avutil.h>
#include <libavutil/mathematics.h>
#endif
#ifdef HAVE_FFMPEG_AVFORMAT_H
#include <ffmpeg/avformat.h>
#else
#include <libavformat/avformat.h>
#endif
#ifdef HAVE_FFMPEG_SWSCALE_H
#include <ffmpeg/swscale.h>
#else
#include <libswscale/swscale.h>
#endif
}

#if !defined(HAVE_AVMEDIA_TYPE_VIDEO)
# define AVMEDIA_TYPE_VIDEO CODEC_TYPE_VIDEO
# define AVMEDIA_TYPE_AUDIO CODEC_TYPE_AUDIO
# define AV_PKT_FLAG_KEY PKT_FLAG_K