cmake_minimum_required(VERSION 3.0.0)
project(test)

include(CTest)
enable_testing()

add_subdirectory(testJson)
add_test(NAME testJson COMMAND testJson)

# add_subdirectory(testSHA1)
# add_test(NAME testSHA1 COMMAND testSHA1)

# add_subdirectory(testDES)
# add_test(NAME testDES COMMAND testDES)

# add_subdirectory(testRSA_OS)
# add_test(NAME testRSA_OS COMMAND testRSA_OS)

# add_subdirectory(testLog)
# add_test(NAME testLog COMMAND testLog)