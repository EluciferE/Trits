cmake_minimum_required(VERSION 3.17)
project(Trits)

add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)

add_executable(Trits main.cpp trit.h tritset.cpp tritset.cpp tritset.cpp tritset.h trit.cpp tests.cpp)
target_link_libraries(Trits gtest gtest_main)