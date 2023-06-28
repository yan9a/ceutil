#!/bin/bash

SCRIPTNAME=`basename "$0"`
PRJNAME="${SCRIPTNAME%.*}"
SCRIPTDIR="${0%/$PRJNAME.*}"
echo "Project: $PRJNAME"
echo "Script directory: $SCRIPTDIR"

if [ $# == 1 ]; then
 opt_sh=$1
else 
 echo "You can input argument:"
 echo " 'cmake' : to generate cmake files, build, and install"
 echo " 'build': to build and run"
 echo " ..."
 read -p "Input an option: " opt_sh
fi

if [[ "$opt_sh" == "" ]]; then
    opt_sh="build"
fi

echo "Option: $opt_sh"
echo " ."
echo " ."
echo " ."

cd $SCRIPTDIR
if [[ "$opt_sh" == "cmake" ]]; then
    echo "Preparing cmake file"
    if [[ ! -d "./build" ]]; then
        mkdir -p build
    fi
    rm -r ./build/*
    cd build
    cmake -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    ..
    cd ..
    echo " ."
    echo " ."
    echo " ."
fi

if [[ $opt_sh == "cmake" ]] || [[ $opt_sh == "build" ]]; then
    echo "Building ..."
    cd $SCRIPTDIR/build && make # && sudo make install
    if [[ $? == 0 ]]; then
        echo "Build successful"
        echo "Running ..."
        ./$PRJNAME
    else
        echo "Error in compiling"
    fi  
else 
    echo "Running ..."
    ./$PRJNAME
fi


