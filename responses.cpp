//
// Created by Egor Chistyakov on 8/1/20.
//
#include "iostream"
#include "responses.h"

std::ostream &operator<<(std::ostream &os, const BusesForStopResponse &r) {
    if (r.stop.empty()) {
        os << "No stop";
    } else {
        bool first = true;
        for (auto &b : r.buses) {
            if (!first) {
                os << ' ';
            }
            first = false;
            os << b;
        }
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const StopsForBusResponse &r) {
    // Реализуйте эту функцию
    if (r.bus.empty()) {
        os << "No bus";
    } else {
        bool firstStop = true;
        for (const auto &[stop, buses] : r.interchanges) {
            if (!firstStop) {
                std::cout << std::endl;
            }
            firstStop = false;
            os << "Stop " << stop << ":";
            if (buses.empty()) {
                os << " no interchange";
            } else {
                for (const auto &b : buses) {
                    os << ' ' << b;
                }
            }
        }
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const AllBusesResponse &r) {
    if (r.buses_to_stops.empty()) {
        os << "No buses";
    } else {
        bool firstSting = true;
        for (const auto &[bus, stops] : r.buses_to_stops) {
            if (!firstSting) {
                os << std::endl;
            }
            firstSting = false;
            os << "Bus " << bus << ":";
            for (const auto &s : stops) {
                os << ' ' << s;
            }
        }
    }
    return os;
}
