dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(crack-attack,1.1.10,lorien420@myrealbox.com)
AC_CONFIG_SRCDIR(src/Attack.cxx)
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(src/config.h)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

dnl Checks for libraries.
# FIXME: Replace `main' with a function in `-lGL':
AC_CHECK_LIB([GL], [glBegin])
# FIXME: Replace `main' with a function in `-lGLU':
AC_CHECK_LIB([GLU], [gluLookAt])
# I don't know how to check these.
# FIXME: Replace `main' with a function in `-lX11':
#AC_CHECK_LIB([X11], [main])
# FIXME: Replace `main' with a function in `-lXi':
#AC_CHECK_LIB([Xi], [main])
# FIXME: Replace `main' with a function in `-lXmu':
#AC_CHECK_LIB([Xmu], [main])
# FIXME: Replace `main' with a function in `-lglut':
AC_CHECK_LIB([glut], [glutInit])

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h string.h sys/socket.h unistd.h])

dnl Check for gtk
AC_ARG_ENABLE(gtk,
  AC_HELP_STRING([--enable-gtk=[yes/no]],[Use gtk front-end [default=yes]]),
  enable_gtk="$enableval",
  enable_gtk=yes)
if test "$enable_gtk" = "yes"; then
  pkg_modules="gtk+-2.0 >= 2.0.0"
  PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
  AC_DEFINE([WANT_GTK], [], [Wants to compile gtk front-end])
else
  PACKAGE_CFLAGS=""
  PACKAGE_LIBS=""
fi
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

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
AC_CHECK_FUNCS([atexit gethostbyname inet_ntoa mkdir pow socket sqrt strchr strcspn strstr poll])

AC_CONFIG_FILES(
  Makefile 
  src/Makefile 
  src/gtk-gui/Makefile
  data/Makefile 
  doc/Makefile)

AC_OUTPUT
