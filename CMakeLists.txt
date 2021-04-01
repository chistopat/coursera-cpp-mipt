cmake_minimum_required(VERSION 3.16)
project(yellow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")
add_executable(yellow main.cpp test_runner.h profile.h)
