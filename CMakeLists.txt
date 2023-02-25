cmake_minimum_required(VERSION 3.0.0)
project (ceUtil VERSION 1.0.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "Project dir = ${PROJECT_SOURCE_DIR}")
message(STATUS "Project version = ${CMAKE_PROJECT_VERSION}")
set(ADDCV YES)
if(WIN32)
    # For windows -------------------------------------------------------
    # Set VCPKGDIR environmental variable for vcpkg root folder
    message(STATUS "vcpkg dir = $ENV{VCPKGDIR}")
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        # if 32 bits
        message(STATUS "Generator x86-windows")
        include_directories($ENV{VCPKGDIR}/installed/x86-windows/include)
        link_directories($ENV{VCPKGDIR}/installed/x86-windows/lib)
    else()
        # if 64 bits
        message(STATUS "Generator x64-windows")
        include_directories($ENV{VCPKGDIR}/installed/x64-windows/include)
        link_directories($ENV{VCPKGDIR}/installed/x64-windows/lib)
    endif()
    find_package(jsoncpp REQUIRED)
    set(JSONCPP_LIBRARIES jsoncpp)
    find_package(wxWidgets COMPONENTS core net base REQUIRED)
    if(ADDCV)
        find_package(OpenCV REQUIRED)
    endif()
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
endif()
#  ------------------------------------------------------------------
if(ADDCV)
    set(LIBS ${JSONCPP_LIBRARIES} ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})
else()
    set(LIBS ${JSONCPP_LIBRARIES} ${wxWidgets_LIBRARIES})
endif()
message(STATUS "List of libraries = ${LIBS}")
include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(${PROJECT_NAME}
${CMAKE_SOURCE_DIR}/src/ceDateTime.cpp
${CMAKE_SOURCE_DIR}/src/ceMmDateTime.cpp
${CMAKE_SOURCE_DIR}/src/ceLog.cpp
${CMAKE_SOURCE_DIR}/src/ceSerial.cpp
${CMAKE_SOURCE_DIR}/src/ceWxTmr.cpp
${CMAKE_SOURCE_DIR}/src/ceUDP.cpp
${CMAKE_SOURCE_DIR}/src/ceMisc.cpp
${CMAKE_SOURCE_DIR}/src/ceFrame.cpp
${CMAKE_SOURCE_DIR}/src/ceFraChecksum.cpp
${CMAKE_SOURCE_DIR}/src/ceFraCmd.cpp
${CMAKE_SOURCE_DIR}/src/ceI2C.cpp
${CMAKE_SOURCE_DIR}/src/cePCA9535.cpp
${CMAKE_SOURCE_DIR}/src/ceConfig.cpp
${CMAKE_SOURCE_DIR}/src/ceModbus.cpp
${CMAKE_SOURCE_DIR}/src/ceWxSerial.cpp
${CMAKE_SOURCE_DIR}/src/cewxcvMisc.cpp
)
target_link_libraries(${PROJECT_NAME} ${LIBS})
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ce
        DESTINATION include 
        FILES_MATCHING 
        PATTERN "*.h" 
)