cmake_minimum_required(VERSION 3.0.0)

# directory name to be used as project name
project (testJson VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})
include_directories(${JSONCPP_INCLUDE_DIRS})
find_package(wxWidgets COMPONENTS core net base REQUIRED)
include("${wxWidgets_USE_FILE}")
#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIR})
include_directories( ${PROJECT_SOURCE_DIR}/../../autoqa/include )

#include_directories( ${PROJECT_SOURCE_DIR}/../../src )
add_executable(${PROJECT_NAME} 
${PROJECT_SOURCE_DIR}/main.cpp
# ${PROJECT_SOURCE_DIR}/../../autoqa/src/MsgCodec.cpp
)

target_link_libraries(${PROJECT_NAME}
ceUtil
${JSONCPP_LIBRARIES} 
${wxWidgets_LIBRARIES}
#${OpenCV_LIBS}
)
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

