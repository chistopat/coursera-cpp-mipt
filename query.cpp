#include "query.h"

std::istream &operator>>(std::istream &is, Query &q) {
    const std::map<std::string, QueryType> QueryTypeMap = {
            {"NEW_BUS", QueryType::NewBus},
            {"BUSES_FOR_STOP", QueryType::BusesForStop},
            {"STOPS_FOR_BUS", QueryType::StopsForBus},
            {"ALL_BUSES", QueryType::AllBuses},
    };
    std::string queryTypeString;
    is >> queryTypeString;
    q.type = QueryTypeMap.at(queryTypeString);
    switch (q.type) {
        case QueryType::NewBus: {
            is >> q.bus;
            size_t stopsCount;
            is >> stopsCount;
            q.stops.resize(stopsCount);
            for (auto &s : q.stops) {
                is >> s;
            }
            break;
        }
        case QueryType::BusesForStop:
            is >> q.stop;
            break;
        case QueryType::StopsForBus:
            is >> q.bus;
            break;
        case QueryType::AllBuses:
            break;
    }
    return is;
}
