#!/bin/bash
#
if [ "x$SIMPATH" == "x" ]; then
# check if FairSoftInst exists one level up
 if [ -d $PWD/../FairSoftInst ]; then
   export SIMPATH=$PWD/../FairSoftInst
 else
    echo "*** No FairSoft installation directory is defined."
    echo "*** Please set the environment variable SIMPATH to the Fairsoft installation directory."
    exit 1
 fi 
fi

# if on lxplus
distribution=$(lsb_release -is)
version=$(lsb_release -rs | cut -f1 -d.)     

if [ "$distribution$version" = "ScientificCERNSLC6" ]; then
 # operating system of last century, need newer gcc / python
  eval "$(/afs/cern.ch/sw/lcg/releases/lcgenv/latest/lcgenv -p /afs/cern.ch/sw/lcg/releases/LCG_85 x86_64-slc6-gcc49-opt Python)"
fi

if [ ! -d build ]; then
 mkdir build
fi
installDir="${PWD/FairRoot/FairRootInst}"
cd build

export PATH=$SIMPATH/bin:$PATH
if [ "$distribution$version" = "ScientificCERNSLC6" ]; then
 xx=$($SIMPATH/bin/fairsoft-config --cxx)
 yy=$($SIMPATH/bin/fairsoft-config --cc)
 cmake .. -DCMAKE_INSTALL_PREFIX=$installDir -DCMAKE_CXX_COMPILER=$xx -DCMAKE_C_COMPILER=$yy
else
 cmake .. -DCMAKE_INSTALL_PREFIX=$installDir
fi 
make
make install
make test
