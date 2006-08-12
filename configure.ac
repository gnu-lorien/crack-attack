dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(crack-attack,1.1.15-cvs,lorien420@myrealbox.com)
AC_CONFIG_SRCDIR(src/Attack.cxx)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE

AM_CONFIG_HEADER(src/config.h)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

dnl Checks for libraries.
AC_CHECK_LIB([GL], [glBegin])
AC_CHECK_LIB([GLU], [gluLookAt])
# I don't know how to check these.
# FIXME: Replace `main' with a function in `-lX11':
#AC_CHECK_LIB([X11], [main])
# FIXME: Replace `main' with a function in `-lXi':
#AC_CHECK_LIB([Xi], [main])
AC_CHECK_LIB([glut], [glutInit])

AC_CHECK_LIB(SDL_image, IMG_LoadPNG_RW, ,
AC_MSG_ERROR([ *** Unable to find SDL_image library with PNG support
(http://www.libsdl.org/projects/SDL_image/)]))

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h string.h sys/socket.h unistd.h sys/types.h stdlib.h])
AC_CHECK_HEADER(inttypes.h, break, echo "Must have inttypes.h for enet. Exiting..."; exit)

dnl enet checks
AC_CHECK_MEMBER(
  struct msghdr.msg_flags,
  AC_DEFINE([HAS_MSGHDR_FLAGS], [], [has msghdr flags]),
  [],
  [sys/socket.h])

AC_CHECK_TYPE(socklen_t, [AC_DEFINE([HAS_SOCKLEN_T], [], [has socklen t])], ,
              #include <sys/types.h>
              #include <sys/socket.h>
)


dnl BinReloc source additions
AM_BINRELOC
AM_CONDITIONAL([WANT_BINRELOC], [test "$br_cv_binreloc" = "yes"])

dnl Check for gtk
AC_ARG_ENABLE(gtk,
  AC_HELP_STRING([--enable-gtk=[yes/no]],[Use gtk front-end [default=yes]]),
  enable_gtk="$enableval",
  enable_gtk=yes)
if test "$enable_gtk" = "yes"; then
  pkg_modules="gtk+-2.0 >= 2.6.0"
  PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
  AC_DEFINE([WANT_GTK], [], [Wants to compile gtk front-end])
else
  PACKAGE_CFLAGS=""
  PACKAGE_LIBS=""
fi
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)
AM_CONDITIONAL([WANT_GTK], [test "$enable_gtk" = yes])

dnl Check for sound support
AC_ARG_ENABLE(sound,
  AC_HELP_STRING([--enable-sound=[yes/no]],
    [Play sounds and music during game [default=no]]),
  enable_sound="$enableval",
  enable_sound="no")
AUDIO_CFLAGS=""
AUDIO_LIBS=""
if test "$enable_sound" != "no"; then
  found_sound="no"
  AM_PATH_SDL(1.2.0,
    [
      AC_CHECK_LIB([SDL_mixer],
        [Mix_OpenAudio],
        [found_sound=yes],
        [found_sound=no]
      )
    ],[found_sound=no]
  )

  if test "$enable_sound" == "yes" -a "$found_sound" == "no"; then
    AC_MSG_ERROR(libSDL and SDL_mixer are required for sound)
  fi
  if test "$found_sound" == "yes"; then
    AUDIO_CFLAGS="$SDL_CFLAGS"
    AUDIO_LIBS="$SDL_LIBS -lSDL_mixer"
    AC_DEFINE(AUDIO_ENABLED, 1, [Has audio support])
  fi
fi
AC_SUBST(AUDIO_CFLAGS)
AC_SUBST(AUDIO_LIBS)
AM_CONDITIONAL([AUDIO_ENABLED], [test "$found_sound" = "yes"])


dnl Check for debugging
AC_ARG_ENABLE(debug,
  AC_HELP_STRING([--enable-debug=[no/yes]],[Compile extra debugging info [default=no]]),
  enable_debug="$enableval",
  enable_debug=no)
if test "$enable_debug" = "yes"; then
  DEBUG_INCLUDES=-DDEVELOPMENT
  DEBUG_CFLAGS="-g3 -ggdb3 -Wall -pedantic"
else
  DEBUG_INCLUDES=-DNDEBUG
  DEBUG_CFLAGS=
fi
AC_SUBST(DEBUG_INCLUDES)
AC_SUBST(DEBUG_CFLAGS)

# Suggested by davyd, borrowed from Nautilus
AC_ARG_ENABLE(more-warnings,
[  --enable-more-warnings  Maximum compiler warnings],
set_more_warnings="$enableval",[
    set_more_warnings=no
])
AC_MSG_CHECKING(for more warnings, including -Werror)
if test "$GCC" = "yes" -a "$set_more_warnings" != "no"; then
    AC_MSG_RESULT(yes)
    CXXFLAGS="\
    -Wall -Werror \
    -Wchar-subscripts \
    -Wpointer-arith \
    -Wcast-align -Wsign-compare \
    $CXXFLAGS"

    for option in -Wno-strict-aliasing -Wno-sign-compare; do
        SAVE_CXXFLAGS="$CXXFLAGS"
        CXXFLAGS="$CXXFLAGS $option"
        AC_MSG_CHECKING([whether gcc understands $option])
        AC_TRY_COMPILE([], [],
            has_option=yes,
            has_option=no,)
        if test $has_option = no; then
            CXXFLAGS="$SAVE_CXXFLAGS"
        fi
        AC_MSG_RESULT($has_option)
        unset has_option
        unset SAVE_CXXFLAGS
    done
    unset option
else
    AC_MSG_RESULT(no)
fi

dnl Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T

AC_PATH_XTRA

dnl Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MEMCMP
AC_FUNC_STAT
AC_CHECK_FUNCS([atexit gethostbyname gethostbyname_r gethostbyaddr_r fcntl inet_ntoa mkdir pow socket sqrt strchr strcspn strstr poll])

AC_CONFIG_FILES(
  Makefile
  src/Makefile
  src/gtk-gui/Makefile
  data/Makefile
  data/models/Makefile
  doc/Makefile
  autopackage/default.apspec
  enet/Makefile
  enet/include/Makefile
  enet/include/enet/Makefile)

AC_OUTPUT
