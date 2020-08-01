#pragma once

#include "string"
#include "vector"
#include "map"
#include "utility"
#include "sstream"

struct BusesForStopResponse {
    std::string stop;
    std::vector<std::string> buses;
};

struct StopsForBusResponse {
    std::string bus;
    std::vector<std::pair<std::string, std::vector<std::string>>> interchanges;
};

struct AllBusesResponse {
    const std::map<std::string, std::vector<std::string>> &buses_to_stops;
};

std::ostream &operator<<(std::ostream &os, const BusesForStopResponse &r);
std::ostream &operator<<(std::ostream &os, const StopsForBusResponse &r);
std::ostream &operator<<(std::ostream &os, const AllBusesResponse &r);
