#!/bin/bash

# fix script
# sed -i -e 's/\r$//' ceutil.sh
# chmod +x ceutil.sh

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


if [[ "$opt_sh" == "install" ]]; then
    # add the user to groups
    opt_au="y"
    read -p "Do you want to add current user to netdev and dialout groups [y/N]?: " opt_au 
    if [ $opt_au == "y" ] || [ $opt_au == "Y" ]; then
        echo "Adding $USER to groups"
        sudo usermod -a -G netdev $USER
        sudo usermod -a -G dialout $USER
    fi
    opt_reqlib="y"
    read -p "Do you want to install required libraries [y/N]?: " opt_reqlib 
    if [ $opt_reqlib == "y" ] || [ $opt_reqlib == "Y" ]; then
        # install required lib
        echo "Installing required lib ..."        
        sudo apt update \
        && apt install --no-install-recommends --no-install-suggests -y build-essential cmake \
        libgtk-3-dev checkinstall libwxgtk3.0-gtk3-dev \
        git pkg-config libavcodec-dev libavformat-dev libswscale-dev libopencv-dev \
        libjsoncpp-dev apt-transport-https ca-certificates \
        autoconf automake libtool gdb cmake-extras \
        libssl-dev openssl 
    fi
    echo " ."
    echo " ."
    echo " ."
fi # lib

cd $SCRIPTDIR
if [[ "$opt_sh" == "install" ]] || [[ "$opt_sh" == "cmake" ]]; then
    echo "Preparing cmake file"    
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
    cd build && make && sudo make install
    if [[ $? == 0 ]]; then
        echo "Build successful"
        ls -l *ceutil.so
        ldd *ceutil.so
        # ls -l *$ceutil.a
        # ar -t *$ceutil.a
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

    

