//
#pragma once

#include "parse.h"

#include <unordered_map>
#include <string_view>
#include <vector>
#include <string>

class DistanceMap {
public:
    static DistanceMap FromString(std::string_view input) {
        std::unordered_map<std::string, int> distance_map;
        while (!input.empty()) {
            string_view token = Strip(ReadToken(input, ","));
            string_view distance_sv = Strip(ReadToken(token, "to"));
            distance_sv.remove_suffix(1); //remove "m"
            std::string stop_name = string(Strip(token));
            int distance = stoi(string(distance_sv));
            distance_map.insert({stop_name, distance});
        }
        return DistanceMap{distance_map};
    }

    DistanceMap(std::unordered_map<std::string, int> distance_map) : values(std::move(distance_map)) {
    }

    DistanceMap() = default;

    std::unordered_map<std::string, int> values;
};

