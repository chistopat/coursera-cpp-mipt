#pragma once

#include <cmath>
#include <memory>

#include "router.h"

using namespace std;

struct Distance {
    int by_road = 0;
    double by_geo = 0;

    Distance& operator+=(const Distance& other) {
        this->by_geo += other.by_geo;
        this->by_road += other.by_road;
        return *this;
    }
};


class BusRoute {
public:
    static unique_ptr<BusRoute> MakeBusRoute(const string& name,  bool is_roundtrip,
                                             const vector<shared_ptr<Stop>>& stop_list,
                                             const Router& router) {
        BusRoute bus_route(name, is_roundtrip, stop_list, router);
        return make_unique<BusRoute>(move(bus_route));
    }

    BusRoute(const string& name, bool is_roundtrip, const vector<shared_ptr<Stop>>& stop_list, const Router& router)
        : is_roundtrip(is_roundtrip)
        , name(name)
        , total_stops_count(0)
        , unique_stops_count(0)
        , route(stop_list)
        , router_(router)
    {
        ComputeRouteLength();
        SetTotalStopsCount();
        SetUniqueStopsCount();
    }

    string ToString() const {
        ostringstream os;
        os << total_stops_count << " stops on route, "
           << unique_stops_count << " unique stops, "
           << distance_.by_road <<  " route length, "
           << distance_.by_road / distance_.by_geo << " courvature";
        return os.str();
    }

    bool is_roundtrip;
    string name;
    size_t total_stops_count;
    size_t unique_stops_count;
    vector<shared_ptr<Stop>> route;
    Distance distance_;


private:
    const Router& router_;

private:
    void SetTotalStopsCount() {
        if (is_roundtrip) {
            total_stops_count = route.size();
        } else {
            total_stops_count = (route.size() * 2) - 1;
        }
    }

    void SetUniqueStopsCount() {
        set<string> uniq_names;
        for (const auto& stop : route) {
            uniq_names.insert(stop->name);
        }
        unique_stops_count = uniq_names.size();
    }

    void ComputeRouteLength() {
        distance_ = ComputeOneLineLength(route.begin(), route.end());
        if (!is_roundtrip) {
            distance_ += ComputeOneLineLength(route.rbegin(), route.rend());
        }
    }
public:
    static double LengthBetweenPoints(shared_ptr<Stop> left, shared_ptr<Stop> right) {
        if (left->point == right->point) {
            return 0;
        }
        auto result = acos((sin(left->point.latitude) * sin(right->point.latitude)) +
            (cos(left->point.latitude) * cos(right->point.latitude) *
                cos(fabs(left->point.longitude - right->point.longitude)))
        ) * 6371000;
        return result;
    }

    int LengthBetweenPointsByRoad(shared_ptr<Stop> left, shared_ptr<Stop> right) {
        int result = 0;
        if (left->point == right->point) {
            return 0;
        }
        auto distance = router_.GetDistance(left->name, right->name);
        result = distance.has_value() ? distance.value() : 0;
        return result;
    }

    template <typename It>
    Distance ComputeOneLineLength(It begin, It end) {
        Distance distance;
        auto first = begin;
        for (auto it = begin + 1; it != end; ++it) {
            distance.by_geo += LengthBetweenPoints(*first, *it);
            distance.by_road += LengthBetweenPointsByRoad(*first, *it);
            first = it;
        }
        return distance;
    }

};