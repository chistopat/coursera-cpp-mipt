#pragma once

using namespace std;

class Point {
public:
    static constexpr double halfC = 3.14159265358979323846 / 180;
public:
    explicit Point(double lat,  double lon)
        : latitude(lat * halfC)
        , longitude(lon * halfC) {
    }

    double latitude;
    double longitude;
};

bool operator==(const Point& left, const Point& right) {
    return std::tie(left.latitude, left.longitude) == std::tie(right.latitude, right.longitude);
}

ostream& operator<< (ostream& os, const Point& point) {
    os << point.latitude << " " << point.longitude;
    return os;
}
