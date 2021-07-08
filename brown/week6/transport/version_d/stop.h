#pragma once

#include <memory>
#include "json.h"
using namespace std;

class Stop {
public:
    static unique_ptr<Stop> Create(const string& name, double latitude, double longitude) {
        Stop stop(name, latitude, longitude);
        return make_unique<Stop>(move(stop));
    }

public:
    Stop(const string& name, double lat, double lon)
        : name(name)
        , point(lat, lon) {

    }

    map<string, Json::Node> ToJson() const {
        auto result = map<string, Json::Node>{};
        auto jbuses = vector<Json::Node>{};
        jbuses.insert(jbuses.end(), buses.begin(), buses.end());
        result["buses"] = jbuses;
        return result;
    }

    string ToString() const {
        ostringstream os;
        if (buses.empty()) {
            return "no buses";
        }
        os << "buses ";
        bool first = true;
        for (const auto& bus : buses) {
            if (!first) {
                os << " ";
            }
            os << bus;
            first = false;
        }
        return os.str();
    }

public:
    string name;
    Point point;
    set<string> buses;
};

ostream& operator<< (ostream& os, const Stop& stop) {
    os << stop.name << ": " << stop.point.latitude << " " << stop.point.longitude;
    return os;
}