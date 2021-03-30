cmake_minimum_required(VERSION 2.8)
project (ceUtil)
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_BUILD_TYPE Release)
message(STATUS "Base dir = ${CMAKE_SOURCE_DIR}")
find_package(wxWidgets COMPONENTS core net base REQUIRED)
find_package(OpenCV REQUIRED)
include("${wxWidgets_USE_FILE}")
include_directories(${OpenCV_INCLUDE_DIR})
include_directories( ${CMAKE_SOURCE_DIR}/include )
add_library(${PROJECT_NAME} 
${CMAKE_SOURCE_DIR}/src/ceDateTime.cpp
${CMAKE_SOURCE_DIR}/src/ceMmDateTime.cpp
${CMAKE_SOURCE_DIR}/src/ceLog.cpp
${CMAKE_SOURCE_DIR}/src/ceSerial.cpp
${CMAKE_SOURCE_DIR}/src/ceWxTmr.cpp
${CMAKE_SOURCE_DIR}/src/ceUDP.cpp
${CMAKE_SOURCE_DIR}/src/ceMisc.cpp
${CMAKE_SOURCE_DIR}/src/ceFrame.cpp
${CMAKE_SOURCE_DIR}/src/ceI2C.cpp
${CMAKE_SOURCE_DIR}/src/cePCA9535.cpp
${CMAKE_SOURCE_DIR}/src/ceConfig.cpp
${CMAKE_SOURCE_DIR}/src/cewxcvMisc.cpp
${CMAKE_SOURCE_DIR}/src/ceModbus.cpp
)
target_link_libraries(${PROJECT_NAME} jsoncpp ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ce
        DESTINATION include 
        FILES_MATCHING 
        PATTERN "*.h" 
)