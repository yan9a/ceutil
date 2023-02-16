cmake_minimum_required(VERSION 3.0.0)
project(test)

include(CTest)
enable_testing()

add_subdirectory(testJson)
add_test(NAME testJson COMMAND testJson)