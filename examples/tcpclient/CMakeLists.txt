cmake_minimum_required(VERSION 3.15.0)
# directory name to be used as project name
project(tcpclient VERSION 1.0.1)
set(ADDCV NO)
# CMakeM1 checks ADDCV variable whether to include OpenCV
# and return LIBS variable with the list of libraries to link 
# and also set WIN32OPT variable with WIN32 for Windows
include(${PROJECT_SOURCE_DIR}/../../CMakeM1.txt)
add_executable(${PROJECT_NAME} ${WIN32OPT}
${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.cpp
)
target_link_libraries(${PROJECT_NAME} ${LIBS})