dnl Process this file with autoconf to produce a configure script.
AC_INIT(crack-attack,1.1.10)
AC_CONFIG_SRCDIR(src/main.c)
AC_PREREQ(2.57)
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(src/config.h)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

dnl Checks for libraries.

dnl Checks for header files.
AC_STDC_HEADERS
AC_CHECK_HEADERS(limits.h unistd.h sys/socket.h sys/poll.h netinet/in.h netdb.h arpa/inet.h sys/stat.h)

pkg_modules="gtk+-2.0 >= 2.0.0"
PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE

AC_PATH_XTRA

dnl Checks for library functions.
AC_CHECK_FUNCS(socket poll)

AC_OUTPUT(Makefile src/Makefile data/Makefile doc/Makefile)
