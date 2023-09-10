cmake_minimum_required(VERSION 3.0.0)
project(interface VERSION 1.0.1)
set(ADDCV NO)
# CMakeM1 checks ADDCV variable whether to include OpenCV
# and return LIBS variable with the list of libraries to link 
# and also set WIN32OPT variable with WIN32 for Windows
include(${PROJECT_SOURCE_DIR}/../../CMakeM1.txt)
add_executable(${PROJECT_NAME}
${PROJECT_SOURCE_DIR}/src/main.cpp
)
target_link_libraries(${PROJECT_NAME} ${LIBS})
