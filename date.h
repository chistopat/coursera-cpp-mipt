#pragma once

#include "vector"
#include "iostream"
#include "iomanip"

using namespace std;

class Date {
public:
//    Date();
    Date(int year, int month, int day);
    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
    std::vector<int> ToVector() const;
private:
    int _year;
    int _month;
    int _day;
};

Date ParseDate(std::istream& is);

std::ostream& operator<<(std::ostream& stream, const Date& date);
bool operator<(const Date& lhs, const Date& rhs);