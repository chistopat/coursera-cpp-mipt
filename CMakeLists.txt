cmake_minimum_required(VERSION 3.16)
project(yellow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(yellow
        main.cpp test_runner.h profile.h iterator_range.h parse.h parse.cpp search_server.h search_server.cpp)