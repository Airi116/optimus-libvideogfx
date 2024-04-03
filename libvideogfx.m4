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

dnl  for module in .