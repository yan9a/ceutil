cmake_minimum_required(VERSION 2.8.12)
project (testUdp)
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_BUILD_TYPE Release)
message(STATUS "Base dir = ${CMAKE_SOURCE_DIR}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})
include_directories(${JSONCPP_INCLUDE_DIRS})

find_package(wxWidgets COMPONENTS core net base REQUIRED)
include("${wxWidgets_USE_FILE}")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
${CMAKE_SOURCE_DIR}/src/${PROJECT_NAME}.cpp
)
target_link_libraries(${PROJECT_NAME} ceUtil
${wxWidgets_LIBRARIES})
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)