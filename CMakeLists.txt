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

add_executable(comment_server  brown/week2/comment_server/test_runner.h brown/week2/comment_server/main.cpp brown/week4/textures/main.cpp)

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

add_executable(expression brown/week3/expression/main.cpp brown/week3/expression/test_runner.h
        brown/week3/expression/Common.h brown/week3/expression/common.cpp)

add_executable(textures
        brown/week4/textures/main.cpp
        brown/week4/textures/test_runner.h
        brown/week4/textures/Textures.h
        brown/week4/textures/Textures.cpp
        brown/week4/textures/Common.h brown/week4/textures/solution.cpp)

add_executable(forbidden brown/week5/forbidden/forbidden.cpp
        brown/week5/forbidden/test_runner.h)

add_executable(transport_a
        brown/week6/transport/version_a/main.cpp
        brown/week6/transport/version_a/test_runner.h
        brown/week6/transport/version_a/tests.h
        brown/week6/transport/version_a/components.cpp
        brown/week6/transport/version_a/components.h
        brown/week6/transport/version_a/base_request.cpp
        brown/week6/transport/version_a/base_request.h
        brown/week6/transport/version_a/response.cpp
        brown/week6/transport/version_a/response.h)

add_executable(transport_a1
        brown/week6/transport/version_a1/main.cpp
        brown/week6/transport/version_a1/test_runner.h)

add_executable(transport_b
        brown/week6/transport/version_b/main.cpp
        brown/week6/transport/version_b/test_runner.h)

add_executable(transport_c
        brown/week6/transport/version_c/main.cpp
        brown/week6/transport/version_c/test_runner.h brown/week6/transport/version_c/parse.h brown/week6/transport/version_c/point.h brown/week6/transport/version_c/stop.h brown/week6/transport/version_c/bus_route.h brown/week6/transport/version_c/transport_manager.h brown/week6/transport/version_c/request.h brown/week6/transport/version_c/response.h brown/week6/transport/version_c/utils.h brown/week6/transport/version_c/tests.h brown/week6/transport/version_c/segment.h brown/week6/transport/version_c/router.h brown/week6/transport/version_c/distance_map.h)


add_executable(transport_d
        brown/week6/transport/version_d/main.cpp
        brown/week6/transport/version_d/test_runner.h
        brown/week6/transport/version_d/parse.h
        brown/week6/transport/version_d/point.h
        brown/week6/transport/version_d/stop.h
        brown/week6/transport/version_d/bus_route.h
        brown/week6/transport/version_d/transport_manager.h
        brown/week6/transport/version_d/request.h
        brown/week6/transport/version_d/response.h
        brown/week6/transport/version_d/utils.h
        brown/week6/transport/version_d/tests.h
        brown/week6/transport/version_d/segment.h
        brown/week6/transport/version_d/router.h
        brown/week6/transport/version_d/distance_map.h
        brown/week6/transport/version_d/json.h brown/week6/transport/version_d/debug.h)
