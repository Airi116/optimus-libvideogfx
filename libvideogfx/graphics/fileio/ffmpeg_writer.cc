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
  for (int y=0;y<height;y++)
    memcpy(&pict->data[0][y * pict->linesize[0]], yuv.AskFrameY()[y], width);

  for (int y=0;y<height/2;y++)
    memcpy(&pict->data[1][y * pict->linesize[1]], yuv.AskFrameCb()[y], width/2);

  for (int y=0;y<height/2;y++)
    memcpy(&pict->data[2][y * pict->linesize[2]], yuv.AskFrameCr()[y], width/2);
}




FFMPEG_Writer::FFMPEG_Writer()
{
  av_register_all();

  img_convert_ctx = NULL;
}


FFMPEG_Writer::~FFMPEG_Writer()
{
  if (oc) { Close(); }
}

void FFMPEG_Writer::Open(const char* filename, const char* format)
{
  fmt = av_guess_format(format,NULL,NULL);
#ifdef HAVE_AVFORMAT_ALLOW_CONTEXT
  oc = avformat_alloc_context();
#else
  oc = av_alloc_format_context();
#endif
  oc->oformat = fmt;

  mFilename = filename;
}

void FFMPEG_Writer::Close()
{
  av_write_trailer(oc);
  avio_close(oc->pb);

  av_free(oc);

  oc=NULL;
}

int gcd(int a,int b)
{
  while (b!=0)
    {
      int h=a%b;
      a=b;
      b=h;
    }
  return a;
}

int FFMPEG_Writer::AddVideoStream(int w,int h,float fps, int bitrate)
{
  std::cout << "int FFMPEG_Writer::AddVideoStream(" << w << "," << h << "," << fps << "," << bitrate << ")\n";

  AVStream *st;
  st = videoStream = avformat_new_stream(oc, NULL);
  if (!st) {
    std::cerr << "Could not alloc ffmpeg video stream\n";
    return -1;
  }

  st->id = 0;
  int d = roundf(fps*100);
  int n = 100;
  int g = gcd(d,n);

  AVCodecContext *c;
  c = st->codec;
  c->codec_id = fmt->video_codec;
  c->codec_type = AVMEDIA_TYPE_VIDEO;
  c->bit_rate = bitrate;
  c->width  = w;
  c->height = h;
  c->time_base.den = d/g;
  c->time_base.num = n/g;
  c->gop_size = 25;
  c->pix_fmt = PIX_FMT_YUV420P;

  if(oc->oformat->flags & AVFMT_GLOBALHEADER)
    c->flags |= CODEC_FLAG_GLOBAL_HEADER;


  // openVideo()

  AVCodec *codec;

  codec = avcodec_find_encoder(c->codec_id);
  if (!codec) {
    std::cerr << "codec not found\n";
    return -1;
  }

  if (avcodec_