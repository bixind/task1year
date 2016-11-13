cmake_minimum_required(VERSION 3.6)
project(mydeque)

add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

set(SOURCE_FILES deque.h)

add_executable(mydeque main.cpp ${SOURCE_FILES})

add_executable(runTest ${SOURCE_FILES} unittests.cpp)
target_link_libraries(runTest gtest gtest_main)
add_test(mytest runTest)