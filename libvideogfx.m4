# Configure paths for LibVideoGfx
# shamelessly stolen from Owen Taylor

dnl AM_PATH_LIBVIDEOGFX([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl Test for LibVideoGfx, and define LIBVIDEOGFX_CFLAGS and LIBVIDEOGFX_LIBS
dnl
AC_DEFUN(AM_PATH_LIBVIDEOGFX,
[dnl 
dnl Get the cflags and libraries from the libvideogfx-config script
dnl
AC_ARG_WITH(libvideogfx-prefix,[  --with-libvideogfx-prefix=PFX   Prefix where LibVideoGfx is installed (optional)],
            libvideogfx_config_prefix="$withval", libvideogfx_config_prefix="")
AC_ARG_WITH(libvideogfx-exec-prefix,[  --with-libvideogfx-exec-prefix=PFX Exec prefix where LibVideoGfx is installed (optional)],
            libvideogfx_config_exec_prefix="$withval", libvideogfx_config_exec_prefix="")
AC_ARG_ENABLE(libvideogfxtest, [  --disable-libvideogfxtest       Do not try to compile and run a test LibVideoGfx program],
		    , enable_libvideogfxtest=yes)

dnl  for module in . $4
dnl  do
dnl      case "$module" in
dnl         gthread) 
dnl             gtk_config_args="$gtk_config_args gthread"
dnl         ;;
dnl      esac
dnl  done

  if test x$libvideogfx_config_exec_prefix != x ; then
     libvideogfx_config_args="$libvideogfx_config_args --exec-prefix=$libvideogfx_config_exec_prefix"
     if test x${LIBVIDEOGFX_CONFIG+set} != xset ; then
        LIBVIDEOGFX_CONFIG=$libvideogfx_config_exec_prefix/bin/libvideogfx-config
     fi
  fi
  if test x$libvideogfx_config_prefix != x ; then
     libvideogfx_config_args="$libvideogfx_config_args --prefix=$libvideogfx_config_prefix"
     if test x${LIBVIDEOGFX_CONFIG+set} != xset ; then
        LIBVIDEOGFX_CONFIG=$libvideogfx_config_prefix/bin/libvideogfx-config
     fi
  fi

  AC_PATH_PROG(LIBVIDEOGFX_CONFIG, libvideogfx-config, no)
  min_libvideogfx_version=ifelse([$1], ,0.0.0,$1)
  AC_MSG_CHECKING(for LibVideoGfx - version >= $min_libvideogfx_version)
  no_libvideogfx=""
  if test "$LIBVIDEOGFX_CONFIG" = "no" ; then
    no_libvideogfx=yes
  else
    LIBVIDEOGFX_CFLAGS=`$LIBVIDEOGFX_CONFIG $libvideogfx_config_args --cflags`
    LIBVIDEOGFX_LIBS=`$LIBVIDEOGFX_CONFIG $libvideogfx_config_args --libs`
    libvideogfx_config_major_version=`$LIBVIDEOGFX_CONFIG $libvideogfx_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    libvideogfx_config_minor_version=`$LIBVIDEOGFX_CONFIG $libvideogfx_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    libvideogfx_config_micro_version=`$LIBVIDEOGFX_CONFIG $libvideogfx_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_libvideogfxtest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $LIBVIDEOGFX_CFLAGS"
      LIBS="$LIBVIDEOGFX_LIBS $LIBS"
dnl
dnl Now check if the installed LibVideoGfx is sufficiently new. (Also sanity
dnl checks the results of gtk-config to some extent
dnl
dnl      rm -f conf.libvideogfxtest
dnl      AC_TRY_RUN([
dnl #inclu