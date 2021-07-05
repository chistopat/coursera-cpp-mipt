#pragma once

#include <set>
#include <unordered_map>
#include <memory>

#include "distance_map.h"
#include "router.h"

using namespace std;

class TransportManager {
public:
    TransportManager() = default;

    void AddStop(const string& name, double latitude, double longitude, DistanceMap distanceMap = {}) {
        if (!stops_dictonary_.count(name)) {
            stops_dictonary_.insert({name, Stop::Create(name, latitude, longitude)});
            const auto& from = name;
            for (const auto& [to, distance] : distanceMap.values) {
                router_.AddDistance(from, to, distance);
            }
        }
    }

    void AddBus(const string& name, bool is_roundtrip, const vector<string>& stop_list) {
        if (!bus_routes_dictonary_.count(name)) {
            bus_routes_dictonary_.insert(
                {name, BusRoute::MakeBusRoute(name, is_roundtrip, GetStopList(stop_list), router_)});
            for (auto& stop : bus_routes_dictonary_[name]->route) {
                stop->buses.insert(name);
            }
        }
    }

    const optional<Stop> GetStop(const string& name) const {
        if (stops_dictonary_.count(name)) {
            return *stops_dictonary_.at(name);
        }
        return nullopt;
    }

    shared_ptr<BusRoute> GetBus(const string& name) const {
        if (bus_routes_dictonary_.count(name)) {
            return bus_routes_dictonary_.at(name);
        }
        return nullptr;
    }

    vector<shared_ptr<Stop>> GetStopList(const vector<string>& stop_list) const {
        vector<shared_ptr<Stop>> result;
        result.reserve(stop_list.size());
        for (const auto& stop : stop_list) {
            result.push_back(stops_dictonary_.at(stop));
        }
        return result;
    }

private:
    unordered_map<string, shared_ptr<Stop>> stops_dictonary_;
    unordered_map<string, shared_ptr<BusRoute>> bus_routes_dictonary_;
    Router router_;
};