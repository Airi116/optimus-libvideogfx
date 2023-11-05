## Makefile.am for libvideogfx/libvideogfx

if HAVE_X11
SUBDIRS = arch audio utility graphics containers x11 # postscript
else
SUBDIRS = arch audio utility graphics containers # x11 postscript
endif

DIST_SUBDIRS = arch audio utility graphics containers x11

AM_CPPFLAGS = \
	-DDATADIR=\"$(datadir)\"	\
	-DLIBDIR=\"$(libdir)\"		\
	-DSYSCONFDIR=\"$(sysconfdir)\"	\
	-I$(top_srcdir)

#
# libraries to compile and install
#
lib_LTLIBRARIES = libvideogfx.la

# libtool stuff: set version and export symbols for resolving
libvideogfx_includedir = $(includedi