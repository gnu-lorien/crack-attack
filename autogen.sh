#!/bin/sh

aclocal-1.8 $ACLOCAL_FLAGS || exit;
autoheader-2.59 || exit;
automake-1.8 --add-missing --copy;
autoconf-2.59 || exit;
automake-1.8 || exit;
./configure $@

