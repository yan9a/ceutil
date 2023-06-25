# ceutil

Utilities by Cool-Emerald

You can get the ceUtil source from its repository using the following command.

    git clone --depth 1 https://github.com/yan9a/ceutil.git

By default, and the IP address type will be wxIPV6address.
If you want to use wxIPV6address for ceUDP, you can use IPAddress definition at line 21 of ceUDP.h file.

## Linux Setup

After cloning the repository, you can use the following command to install the library.

    ./ceUtil.sh install

If you encounter problems such as "bad interpreter" in running the script, especially when you copied the files from a Windows system, you can try to fix line end and change its mode to be executable as follows.

    sed -i -e 's/\r$//' ceUtil.sh
    chmod +x ceUtil.sh

### Setup Visual Studio Code

There are a few examples in the examples directory to demonstrate about using ceUtil library.

Install required tools as follows.

    sudo apt update
    sudo apt-get install build-essential gdb cmake cmake-extras
    sudo snap install code

Go to a directory inside examples in the cloned repository and enter

    code .

to open visual studio code. Install the following extensions on code IDE.

    * C/C++, Microsoft, C/C++ IntelliSense, debugging, and code browsing.
    * C/C++ Extension Pack, Microsoft, Popular extensions for C++ development in Visual Studio Code.
    * CMake Tools, Microsoft, Extended CMake support in Visual Studio Code

Press ctrl+shift+p to open command pallete and choose the following commands.

    CMake: Configure
    CMake: Build
    CMake: Debug

## Windows Setup


### Setup Visual Studio  

Get the latest visual studio community edition at 

    https://visualstudio.microsoft.com/downloads/

and install tools for C++.

For example, the following components can be selected.

 * MSVC vXXX - VS 20XX C++ x64/x86 build tool...
 * C++ CMake tools for Windows

### Setup vcpkg

We can use vcpkg to install required libraries for ceUtil. Install vcpkg and setup required libraries as follows.
    
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    .\bootstrap-vcpkg.bat
    .\vcpkg integrate install
    .\vcpkg search opencv
    .\vcpkg install opencv:x64-windows
    .\vcpkg install jsoncpp:x64-windows
    .\vcpkg install wxwidgets:x64-windows
    .\vcpkg install openssl:x64-windows

You might need to add the preprocessor definition WXUSINGDLL if the project did not previously reference wxWidgets.

### Setup ceutil

After cloning the ceutil repository, add an environmental variable called 'CEDIR' for the cloned directory path. For example, if you cloned the repository at "C:\Users\aye\ws\ceutil" and find the ceUtil.sh script file in that folder. You can define CEDIR as system variables in "Environment Variables"

    System variables
      Variable          Value
      CEDIR             C:\Users\aye\ws\ceutil    


Open visual studio solution in the ceutil sub-directory and build for x64 release and debug configurations.


### Open an Example Project

After setting up the requried libraries, you can now open an example solution in examples directory of the cloned repository.

### Docker

Docker image 'yan9a/cebian' has all the required libraries for ceUtil setup. Its Dockerfile can also be found in the root folder.