#!/bin/bash

MAKE='make clean && make && make install'
echo "Running autogen.sh"
./autogen.sh --prefix=`pwd`/inst &> /dev/null
# Default build test
echo "Testing default build"
(./configure --prefix=`pwd`/inst \
&& make clean && make && make install) &> /dev/null
echo "Game should start with GUI"
./inst/bin/crack-attack > /dev/null
echo "Ensuring relocatable..."
mv ./inst ./moved_inst
echo "Game should start with GUI"
./moved_inst/bin/crack-attack > /dev/null
echo "Game should start in a solo game"
./moved_inst/bin/crack-attack --solo > /dev/null
mv ./moved_inst ./inst

echo "Testing no gui"
(./configure --prefix=`pwd`/inst --disable-gtk \
&& make clean && make && make install) &> /dev/null
echo "Game should fail to start [and show usage]"
./inst/bin/crack-attack > /dev/null
echo "Game should start with solo low extreme game"
./inst/bin/crack-attack -1 -X > /dev/null

echo "Testing no relocatable"
(./configure --prefix=`pwd`/inst --disable-binreloc \
&& make clean && make && make install) &> /dev/null
echo "Game should start with solo low extreme game"
./inst/bin/crack-attack -1 -X > /dev/null
mv ./inst ./moved_inst
echo "Game should fail to start [due to missing shared files]"
./moved_inst/bin/crack-attack -1 -X > /dev/null
mv ./moved_inst ./inst

echo "Testing for Gentoo \"bump\" ability"
rm -rf ./inst
(./configure --prefix=`pwd`/inst --bindir=`pwd`/inst/mybin \
--datadir=`pwd`/inst/sharingiscaring --disable-binreloc \
&& make clean && make && make install) &> /dev/null
echo "Game should start with solo low extreme game"
./inst/mybin/crack-attack -1 -X > /dev/null
echo "GUI should start. Make sure to start a solo game"
./inst/mybin/crack-attack > /dev/null
