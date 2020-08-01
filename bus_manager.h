#pragma once

#include "string"
#include "vector"
#include "responses.h"

class BusManager {
public:
    void AddBus(const std::string& bus, const std::vector<std::string>& stops) ;

    [[nodiscard]] BusesForStopResponse GetBusesForStop(const std::string& stop) const ;

    [[nodiscard]] StopsForBusResponse GetStopsForBus(const std::string& bus) const;

    [[nodiscard]] AllBusesResponse GetAllBuses() const ;

private:
    std::map<std::string, std::vector<std::string>> busToStopsMap, stopToBusesMap;
};
