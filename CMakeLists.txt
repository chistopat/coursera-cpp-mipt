cmake_minimum_required(VERSION 3.16)
project(yellow)

set(CMAKE_CXX_STANDARD 17)

#brown
add_executable(hashtable common/test_runner.h common/profile.h brown/week1/hashtable/main.cpp)