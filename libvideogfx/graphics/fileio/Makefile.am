## Makefile.am for libvideogfx/libvideogfx/graphics/fileio

# SUBDIRS = avi

noinst_LTLIBRARIES = libvideogfx-graphics-fileio.la

libvideogfx_graphics_fileio_includedir = \
	$(includedir)/libvideogfx/graphics/fileio

if LINUX
linux_sources = \
	v4l.cc	\
	v4l.hh	\
	mplayer.hh \
	mplayer.cc

linux_headers = \
	v4l.hh \
	mplayer.hh
else
linux_sources =
linux_headers =
endif

if HAVE_FFMPEG
ffmpeg_sources = ffmpeg.cc ffmpeg_writer.cc
ffmpeg_headers = ffmpeg.hh ffmpeg_writer.hh
else
ffmpeg_sources = 
ffmpeg_headers = 
endif

libvideogfx_graphics_fileio_la_SOURCES = \
	base.hh \
	jpeg.cc	\
	jpeg.hh	\
	png.cc	\
	png.hh	\
	uyvy.cc	\
	uyvy.hh	\
	ppm.cc	\
	ppm.hh  \
	raw.hh  \
	raw.cc  \
	yuv.cc	\
	yuv.h