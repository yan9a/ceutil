cmake_minimum_required(VERSION 3.0.0)
project(Timer)

set(ADDCV NO)
# CMakeM1 checks ADDCV variable whether to include OpenCV
# and return LIBS variable with the list of libraries to link 
include(${PROJECT_SOURCE_DIR}/../../CMakeM1.txt)

add_executable(${PROJECT_NAME}
${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.cpp
)

target_link_libraries(${PROJECT_NAME}
${LIBS}
)

# install(TARGETS ${PROJECT_NAME} DESTINATION bin)
# install(FILES ${PROJECT_SOURCE_DIR}/conf.json
#         DESTINATION etc/${PROJECT_NAME})
