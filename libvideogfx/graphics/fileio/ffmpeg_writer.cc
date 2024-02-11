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
# define AV_PKT_FLAG_KEY PKT_FLAG_KEY
# define AV_SAMPLE_FMT_S16 SAMPLE_FMT_S16
# define AVIO_FLAG_WRITE URL_WRONLY
#endif

#ifndef HAVE_AVIO_OPEN
#define avio_open url_fopen
#ifdef HAVE_AVFORMAT_CONTEXT_BYTEIOCONTEXT_POINTER
#  define avio_close(x) url_fclose(x)
#else
#  define avio_close(x) url_fclose(&(x))
#endif
#define av_dump_format dump_format
#define av_guess_format guess_format
#endif

#include <unistd.h>


namespace videogfx
{

static AVFrame *allocPicture(enum PixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    uint8_t *picture_buf;
    int size;

    picture = avcodec_alloc_frame();
    if (!picture)
        return NULL;
    size = avpicture_get_size(pix_fmt, width, height);
    picture_buf = (uint8_t*)av_malloc(size);
    if (!picture_buf) {
        av_free(picture);
        return NULL;
    }
    avpicture_fill((AVPicture *)picture, picture_buf,
                   pix_fmt, width, height);
    return picture;
}


static void fillImage(AVFrame *pict, int width, int height, const Image<Pixel>& yuv)
{
  