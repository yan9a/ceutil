#!/bin/bash

SCRIPTNAME=`basename "$0"`
PRJNAME="${SCRIPTNAME%.*}"
SCRIPTDIR="${0%/$PRJNAME.*}"
echo "Project: $PRJNAME"
echo "Script directory: $SCRIPTDIR"

opt_sh="build"

if [ $# == 1 ]; then
 opt_sh=$1
else 
 echo "You can input argument:"
 echo " 'install' : to install prerequisite packages, generate cmake files, build and install the lib"
 echo " 'cmake' : to generate cmake files, build, and install"
 echo " 'build': to build and install"
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


if [[ "$opt_sh" == "install" ]]; then
    # add the user to groups
    # echo "Adding $USER to groups"
    # sudo usermod -a -G netdev $USER
    # sudo usermod -a -G dialout $USER

    # install required lib
    echo "Installing required lib ..."
    # sudo apt update
    sudo apt -y install build-essential cmake 

    # for wxWidgets prerequisite
    sudo apt -y install libgtk-3-dev checkinstall
    sudo apt -y install libwxgtk3.0-gtk3-dev
    wx-config --version

    # opencv prerequisite
    sudo apt -y install git pkg-config libavcodec-dev libavformat-dev libswscale-dev
    sudo apt -y install libopencv-dev
    # pkg-config --modversion opencv
    # sudo sh -c "echo /usr/local/lib/ > /etc/ld.so.conf.d/opencv.conf"
    # sudo ldconfig

    #jsoncpp
    sudo apt -y install libjsoncpp-dev     
    # sudo sh -c "echo /usr/local/lib/ > /etc/ld.so.conf.d/jsoncpp.conf"
    # sudo ldconfig

    echo " ."
    echo " ."
    echo " ."
fi # lib

if [[ "$opt_sh" == "install" ]] || [[ "$opt_sh" == "cmake" ]]; then
    echo "Preparing cmake file"
    cd $SCRIPTDIR
    if [[ ! -d "./build" ]]; then
        mkdir -p build
    fi
    rm -r ./build/*
    cd build
    cmake -D CMAKE_BUILD_TYPE=Release \
    -D BUILD_SHARED_LIBS=ON \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    ..
    cd ..
    echo " ."
    echo " ."
    echo " ."
fi

if [[ $opt_sh == "install" ]] || [[ $opt_sh == "cmake" ]] || [[ $opt_sh == "build" ]]; then
    echo "Building ..."
    cd $SCRIPTDIR/build && make && sudo make install
    if [[ $? == 0 ]]; then
        echo "Build successful"
        ls -l *ceUtil.so
        ldd *ceUtil.so
        # ls -l *$ceUtil.a
        # ar -t *$ceUtil.a
    else
        echo "Error in compiling"
    fi  
else 
    echo "Invalid option for $PRJNAME ..."
fi

if [[ "$opt_sh" == "install" ]]; then
    echo "Configuring lib path ..."
    sudo sh -c "echo /usr/local/lib/ > /etc/ld.so.conf.d/ceutil.conf"
    sudo ldconfig
    echo " ."
    echo " ."
    echo " ."
fi # config

    

