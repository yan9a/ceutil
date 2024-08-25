cmake_minimum_required(VERSION 3.5.0)
project (ceutil VERSION 2.1.3)
# version 1.0.0 Added DES, SHA, RSA
# version 2.0.0 Replace wxAppConsole with wxEvtHandler, add TCP server, header only

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "Project dir = ${PROJECT_SOURCE_DIR}")
message(STATUS "Project version = ${CMAKE_PROJECT_VERSION}")
set(ADDCV YES)
if(WIN32)
    # For windows -------------------------------------------------------
    # Set VCPKG_ROOT environmental variable for vcpkg root folder
    message(STATUS "vcpkg dir = $ENV{VCPKG_ROOT}")
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        # if 32 bits
        message(STATUS "Generator x86-windows")
        include_directories($ENV{VCPKG_ROOT}/installed/x86-windows/include)
        link_directories($ENV{VCPKG_ROOT}/installed/x86-windows/lib)
    else()
        # if 64 bits
        message(STATUS "Generator x64-windows")
        include_directories($ENV{VCPKG_ROOT}/installed/x64-windows/include)
        link_directories($ENV{VCPKG_ROOT}/installed/x64-windows/lib)
    endif()
    find_package(jsoncpp REQUIRED)
    set(JSONCPP_LIBRARIES jsoncpp)
    find_package(wxWidgets COMPONENTS core net base REQUIRED)
    if(ADDCV)
        find_package(OpenCV REQUIRED)
    endif()
    find_package(OpenSSL REQUIRED)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    # For linux  -------------------------------------------------------
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JSONCPP jsoncpp)
    link_libraries(${JSONCPP_LIBRARIES})
    include_directories(${JSONCPP_INCLUDE_DIRS})
    find_package(wxWidgets COMPONENTS core net base REQUIRED)
    include("${wxWidgets_USE_FILE}")
    if(ADDCV)
        find_package(OpenCV REQUIRED)
        include_directories(${OpenCV_INCLUDE_DIR})
    endif()
    find_package(OpenSSL REQUIRED)
endif()
#  ------------------------------------------------------------------
set(LIBS 
    ${JSONCPP_LIBRARIES} 
    ${wxWidgets_LIBRARIES}
    OpenSSL::SSL OpenSSL::Crypto
)
if(ADDCV)
    set(LIBS ${LIBS} ${OpenCV_LIBS})
endif()
message(STATUS "List of libraries = ${LIBS}")
include_directories(${PROJECT_SOURCE_DIR}/include)
# add_library(${PROJECT_NAME}

# )
# target_link_libraries(${PROJECT_NAME} ${LIBS})
# install(TARGETS ${PROJECT_NAME} DESTINATION lib)

add_subdirectory(examples/I2C)
add_subdirectory(examples/kbhit)
add_subdirectory(examples/Log)
add_subdirectory(examples/Modbus)
# add_subdirectory(examples/rsakeygen)
add_subdirectory(examples/Serial)
add_subdirectory(examples/tcpclient)
add_subdirectory(examples/tcpsvr)
add_subdirectory(examples/UDP)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ce
        DESTINATION include 
        FILES_MATCHING 
        PATTERN "*.h" 
)
