#include "test_runner.h"

#include "unordered_map"
#include "memory"
#include "optional"

using namespace std;

class Stop {
public:
    static unique_ptr<Stop> Create(const string& name, double latitude, double longitude) {
        Stop stop(name, latitude, longitude);
        return make_unique<Stop>(move(stop));
    }

public:
    Stop(const string& name, double latitude, double longitude)
        : name(name)
        , latitude(latitude)
        , longitude(longitude) {}

public:
    string name;
    double latitude;
    double longitude;
};

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
        , total_stops_count(0)
        , unique_stops_count(0)
        , route(stop_list)
    {
        SetTotalStopsCount();
        SetUniqueStopsCount();
    }

    string ToString() {
        ostringstream os;
        os << total_stops_count << " stops on route, "
           << unique_stops_count << " unique stops, "
           << length <<  " route length";
        return os.str();
    }

    bool is_roundtrip;
    string name;
    double length;
    size_t total_stops_count;
    size_t unique_stops_count;
    vector<shared_ptr<Stop>> route;

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
};

class TransportManager {
public:
    TransportManager() = default;

    void AddStop(const string& name, double latitude, double longitude) {
        if (!stops_dictonary_.count(name)) {
            stops_dictonary_.insert({name, Stop::Create(name, latitude, longitude)});
        }
    }

    void AddBus(const string& name, bool is_roundtrip, const vector<string>& stop_list) {
        if (!bus_routes_dictonary_.count(name)) {
            bus_routes_dictonary_.insert({name, BusRoute::MakeBusRoute(name, is_roundtrip, GetStopList(stop_list))});
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
};

namespace Tests {
    void AddStop() {
        TransportManager transport_manager;
        transport_manager.AddStop("a", 1.0, 0.0);
        const auto a = transport_manager.GetStop("a");

        ASSERT_EQUAL(a->name, "a")
        ASSERT_EQUAL(a->latitude, 1.0)
        ASSERT_EQUAL(a->longitude, 0.0 )
    }

    void AddBus() {
        TransportManager transport_manager;
        transport_manager.AddStop("a", 1.0, 0.0);
        transport_manager.AddStop("b", 1.0, 0.0);
        transport_manager.AddStop("c", 1.0, 0.0);

        transport_manager.AddBus("759", true, {"a", "b", "c", "a"});
        transport_manager.AddBus("000", false, {"a", "b", "c"});

        const auto bus = transport_manager.GetBus("759");
        const auto bus2 = transport_manager.GetBus("000");

        ASSERT_EQUAL(bus->ToString(), "4 stops on route, 3 unique stops, 0 route length")
        ASSERT_EQUAL(bus2->ToString(), "5 stops on route, 3 unique stops, 0 route length")

    }
}

void RunAll() {
    TestRunner test_runner;
    RUN_TEST(test_runner, Tests::AddStop);
    RUN_TEST(test_runner, Tests::AddBus);
}

int main() {
    RunAll();
    return 0;
}