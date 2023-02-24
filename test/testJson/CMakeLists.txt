cmake_minimum_required(VERSION 3.0.0)

# directory name to be used as project name
project (testJson VERSION 1.0.1)
set(ADDCV NO)
# CMakeM1 checks ADDCV variable whether to include OpenCV
# and return LIBS variable with the list of libraries to link 
include(${PROJECT_SOURCE_DIR}/../../CMakeM1.txt)
#include_directories( ${PROJECT_SOURCE_DIR}/src )
add_executable(${PROJECT_NAME} 
${PROJECT_SOURCE_DIR}/main.cpp
)
target_link_libraries(${PROJECT_NAME} ${LIBS})


