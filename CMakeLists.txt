cmake_minimum_required(VERSION 3.16)
project(yellow)

set(CMAKE_CXX_STANDARD 17)

add_executable(yellow main.cpp test_runner.h test_runner.cpp tests.h tests.cpp query.cpp query.h responses.cpp responses.h bus_manager.cpp bus_manager.h)