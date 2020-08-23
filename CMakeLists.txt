cmake_minimum_required(VERSION 3.16)
project(yellow)

set(CMAKE_CXX_STANDARD 17)

add_executable(yellow main.cpp test_runner.h condition_parser_test.cpp condition_parser.cpp condition_parser.h token.cpp token.h node.cpp node.h date.cpp date.h database.cpp database.h test_db.h)