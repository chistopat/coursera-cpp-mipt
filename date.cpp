#include "date.h"

Date::Date(int year, int month, int day)
        : _year(year), _month(month), _day(day) {
    DateValidate();
};


int Date::GetDay() const {
    return _day;
}


int Date::GetMonth() const {
    return _month;
}


int Date::GetYear() const {
    return _year;
}


std::vector<int> Date::ToVector() const {
    return {_year, _month, _day};
}

Date::Date(vector<int> v)
    : _year(v[0]), _month(v[1]), _day(v[2]){
    DateValidate();
}

void Date::DateValidate() {
    if (_month < 1 or _month > 12) {
        throw std::invalid_argument("Month value is invalid: "
                                    + std::to_string(_month));
    }
    if (_day < 1 or _day > 31) {
        throw std::invalid_argument("Day value is invalid: "
                                    + std::to_string(_day));
    }
}


Date ParseDate(std::istream &stream) {
    int year = 0;
    int month = 0;
    int day = 0;
    char dlm1 = '-';
    char dlm2 = '-';
    stream >> year >> dlm1 >> month >> dlm2 >> day;
    return Date(year, month, day);
}

std::ostream& operator<<(std::ostream& stream, const Date& date) {
    stream << setw(4) << setfill('0') << date.GetYear() << '-'
           << setw(2) << setfill('0') << date.GetMonth() << '-'
           << setw(2) << setfill('0') << date.GetDay();
    return stream;
}

bool operator<(const Date& lhs, const Date& rhs){
    if(lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        } else {
            return lhs.GetMonth() < rhs.GetMonth();
        }
    } else {
        return lhs.GetYear() < rhs.GetYear();
    }
}