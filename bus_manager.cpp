//
// Created by Egor Chistyakov on 8/1/20.
//
#include "bus_manager.h"
#include "responses.h"

void BusManager::AddBus(const std::string &bus, const std::vector<std::string> &stops) {
    // Реализуйте этот метод
    busToStopsMap[bus] = stops;
    for (const std::string &stop : stops) {
        stopToBusesMap[stop].push_back(bus);
    }
}

[[nodiscard]] BusesForStopResponse BusManager::GetBusesForStop(const std::string &stop) const {
    if (stopToBusesMap.count(stop) == 0) {
        return {};
    }
    return BusesForStopResponse{stop, stopToBusesMap.at(stop)};
}

[[nodiscard]] StopsForBusResponse BusManager::GetStopsForBus(const std::string &bus) const {
    // Реализуйте этот метод
    if (busToStopsMap.count(bus) == 0) {
        return {};
    }
    StopsForBusResponse response{bus};
    for (const std::string &stop : busToStopsMap.at(bus)) {
        if (stopToBusesMap.at(stop).size() == 1) {
            auto emptyPair = make_pair(stop, std::vector<std::string>{});
            response.interchanges.push_back(emptyPair);
        } else {
            std::vector<std::string> buses;
            for (const std::string &other_bus : stopToBusesMap.at(stop)) {
                if (bus != other_bus) {
                    buses.push_back(other_bus);
                }
            }
            response.interchanges.emplace_back(make_pair(stop, buses));
        }
    }
    return response;
}

[[nodiscard]] AllBusesResponse BusManager::GetAllBuses() const {
    const auto &result = busToStopsMap;
    return AllBusesResponse{result};
}

