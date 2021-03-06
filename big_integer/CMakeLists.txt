cmake_minimum_required(VERSION 3.14)
project(big_integer)

set(CMAKE_CXX_STANDARD 14)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(main main.cpp
        big_integer.h
        big_integer.cpp)

add_executable(big_integer_testing
        big_integer_testing.cpp
        big_integer.h
        big_integer.cpp
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(big_integer_testing -lgmp -lgmpxx -lpthread)