#!/bin/sh

aclocal-1.10 $ACLOCAL_FLAGS || exit;
autoheader || exit;
autoconf || exit;
automake-1.10 --add-missing --copy || exit;
./configure $@

