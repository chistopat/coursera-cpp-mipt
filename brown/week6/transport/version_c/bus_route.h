#pragma once

#include <cmath>
#include <memory>

using namespace std;

class BusRoute {
public:
    static unique_ptr<BusRoute> MakeBusRoute(const string& name,  bool is_roundtrip,
                                             const vector<shared_ptr<Stop>>& stop_list) {
        BusRoute bus_route(name, is_roundtrip, stop_list);
        return make_unique<BusRoute>(move(bus_route));
    }

    BusRoute(const string& name, bool is_roundtrip, const vector<shared_ptr<Stop>>& stop_list)
        : is_roundtrip(is_roundtrip)
        , name(name)
        , length(0)
        , courvature(0)
        , total_stops_count(0)
        , unique_stops_count(0)
        , route(stop_list)
    {
        ComputeRouteLength();
        SetTotalStopsCount();
        SetUniqueStopsCount();
    }

    string ToString() const {
        ostringstream os;
        os << total_stops_count << " stops on route, "
           << unique_stops_count << " unique stops, "
           << length <<  " route length";
        return os.str();
    }

    bool is_roundtrip;
    string name;
    double length;
    double courvature;
    size_t total_stops_count;
    size_t unique_stops_count;
    vector<shared_ptr<Stop>> route;

private:


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
        length = ComputeOneLineLength(route.begin(), route.end());
        if (!is_roundtrip) {
            length += ComputeOneLineLength(route.rbegin(), route.rend());
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

    template <typename It>
    static double ComputeOneLineLength(It begin, It end) {
        double length = 0;
        auto first = begin;
        for (auto it = begin + 1; it != end; ++it) {
            length += LengthBetweenPoints(*first, *it);
            first = it;
        }
        return length;
    }
};