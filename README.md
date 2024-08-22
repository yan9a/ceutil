# ceutil

Utilities by Cool-Emerald

You can get the ceutil source from its repository using the following command.

    git clone --depth 1 https://github.com/yan9a/ceutil.git

By default, and the IP address type for ceUDP will be wxIPV4address.
If you want to use wxIPV6address for it, you can modify IPAddress definition in ceUDP.h file.

## Linux Setup

After cloning the repository, you can use the following command to install the library.

    cd ceutil
    ./ceutil.sh install

If you encounter problems such as "bad interpreter" in running the script, especially when you copied the files from a Windows system, you can try to fix line end and change its mode to be executable as follows.

    sed -i -e 's/\r$//' ceutil.sh
    chmod +x ceutil.sh

Install script prompts the user to add to netdev and dialout groups and also to install required libraries.

### Setup Visual Studio Code

There are a few examples in the examples directory to demonstrate about using ceutil library.

Install visual studio code as follows.

    sudo snap install code

If you are using WSL, visual code is already there and you don't need the above command.
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

### Setup cmake

Get the latest cmake at 

 https://cmake.org/download/

and adding path for it when installing.

### Setup vcpkg

We can use vcpkg to install required libraries for ceutil. Install vcpkg and setup required libraries as follows.
    
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    .\bootstrap-vcpkg.bat
    .\vcpkg integrate install
    .\vcpkg search opencv
    .\vcpkg install opencv:x64-windows
    .\vcpkg install jsoncpp:x64-windows
    .\vcpkg install wxwidgets:x64-windows
    .\vcpkg install openssl:x64-windows

After cloning the vcpkg repository, add an environmental variable called 'VCPKG_ROOT' for the cloned directory path. For example, if you cloned the repository at "C:\Users\username\vcpkg" and find the bootstrap-vcpkg.bat file in that folder. You can define VCPKG_ROOT as system variables in "Environment Variables"

    System variables
      Variable          Value
      VCPKG_ROOT             C:\Users\username\vcpkg   

### Setup ceutil 

#### Visual Studio IDE

Open visual studio solution in the ceutil sub-directory and build for x64 release and debug configurations.
You might need to add the preprocessor definition WXUSINGDLL if the project did not previously reference wxWidgets.



#### Command Prompt

You can also compile the library by opening "Command Prompt" and running ceutilw.bat. Currently, generator for cmake is set as

    -G "Visual Studio 17 2022"

Check with 

    cmake -h 

for other options.

#### Installed Location

Compiled library will be installed under 

    "$(VCPKG_ROOT)\installed\x64-windows\lib"

under respective configuration. It will also install the include files under 

    "$(VCPKG_ROOT)\installed\x64-windows\include".

### Open an Example Project

After setting up the requried libraries, you can now open an example solution in examples directory of the cloned repository.

### Docker

Docker image 'yan9a/cebian' has all the required libraries for ceutil setup. Its Dockerfile can also be found in the root folder.