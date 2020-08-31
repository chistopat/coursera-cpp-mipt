cmake_minimum_required(VERSION 3.16)
project(yellow)

set(CMAKE_CXX_STANDARD 17)

add_executable(yellow main.cpp test_runner.h test_runner.cpp tests.h tests.cpp deque_test.h deque.h deque.cpp)