cmake_minimum_required(VERSION 3.16)
project(yellow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

#brown
add_executable(hashtable  brown/week1/hashtable/test_runner.h brown/week1/hashtable/main.cpp)
add_executable(set_iterator  brown/week1/set_iterator/test_runner.h brown/week1/set_iterator/main.cpp)
add_executable(hash_person  brown/week1/hash_person/test_runner.h brown/week1/hash_person/main.cpp)
add_executable(secondary_index  brown/week1/secondary_index/test_runner.h brown/week1/secondary_index/main.cpp)

add_executable(ini_parser
        brown/week2/ini_parser/test_runner.h
        brown/week2/ini_parser/ini.cpp
        brown/week2/ini_parser/ini.h
        brown/week2/ini_parser/main.cpp)

add_executable(namespace_refactor
        brown/week2/namespace_refactor/test_runner.h
        brown/week2/namespace_refactor/stats_aggregator_test.cpp
        brown/week2/namespace_refactor/stats_aggregator.cpp
        brown/week2/namespace_refactor/main.cpp)

add_executable(double_dispatch
        brown/week2/double_dispatch/test_runner.h
        brown/week2/double_dispatch/game_object.h
        brown/week2/double_dispatch/geo2d.cpp
        brown/week2/double_dispatch/geo2d.h
        brown/week2/double_dispatch/main.cpp)

add_executable(comment_server  brown/week2/comment_server/test_runner.h brown/week2/comment_server/main.cpp)

add_executable(lazy_value brown/week3/lazy_value/main.cpp brown/week3/lazy_value/test_runner.h)

add_executable(const_concurrency_map
        brown/week3/const_concurrency_map/main.cpp
        brown/week3/const_concurrency_map/test_runner.h
        brown/week3/const_concurrency_map/profile.h)

add_executable(polinomial
        brown/week3/polinomial/main.cpp
        common/test_runner.h
        common/profile.h)

add_executable(pipeline
        brown/week3/pipeline/main.cpp
        common/test_runner.h
        common/profile.h)

