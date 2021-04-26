cmake_minimum_required(VERSION 3.16)
project(yellow)

set(CMAKE_CXX_STANDARD 17)

#brown
add_executable(hashtable  brown/week1/hashtable/test_runner.h brown/week1/hashtable/main.cpp)
add_executable(set_iterator  brown/week1/set_iterator/test_runner.h brown/week1/set_iterator/main.cpp)
add_executable(hash_person  brown/week1/hash_person/test_runner.h brown/week1/hash_person/main.cpp)
add_executable(secondary_index  brown/week1/secondary_index/test_runner.h brown/week1/secondary_index/main.cpp)
