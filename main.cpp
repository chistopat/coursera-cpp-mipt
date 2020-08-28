#include "airline_ticket.h"
#include "test_runner.h"
#include "utility"

using namespace std;

#define UPDATE_FIELD(ticket, field, values) { \
    auto it = values.find(#field); \
    if (it != values.end()) { \
        istringstream is(it->second); \
        is >> ticket.field; \
    }\
} // Реализуйте этот макрос, а также необходимые операторы для классов Date и Time

istream& operator>>(istream& is, Date& date) {
    char dlm;
    is >> date.year >> dlm >> date.month >> dlm >> date.day;
    return is;
}
ostream& operator<<(ostream& os, const Date& date);
ostream& operator<<(ostream& os, const Date& date) {
    os << date.year << '-' << date.month << '-' << date.day;
    return os;
}

bool operator==(const Date& lhs, const Date& rhs) {
    return std::tie(lhs.year, lhs.month, lhs.day) == std::tie(rhs.year, rhs.month, rhs.day);
}

bool operator!=(const Date& lhs, const Date& rhs) {
    return std::tie(lhs.year, lhs.month, lhs.day) != std::tie(rhs.year, rhs.month, rhs.day);
}

bool operator<(const Date& lhs, const Date& rhs) {
    return std::tie(lhs.year, lhs.month, lhs.day) < std::tie(rhs.year, rhs.month, rhs.day);
}

bool operator>(const Date& lhs, const Date& rhs) {
    return std::tie(lhs.year, lhs.month, lhs.day) > std::tie(rhs.year, rhs.month, rhs.day);
}

istream& operator>>(istream& is, Time& time) {
    char dlm;
    is >> time.hours >> dlm >> time.minutes;
    return is;
}

ostream& operator<<(ostream& os, const Time& time) {
    os << time.hours << ':' << time.minutes;
    return os;
}

bool operator==(const Time& lhs, const Time& rhs) {
    return std::tie(lhs.hours, lhs.minutes) == std::tie(rhs.hours, rhs.minutes);
}

void TestUpdate() {
    AirlineTicket t;
    t.price = 0;

    const map<string, string> updates1 = {
            {"departure_date", "2018-2-28"},
            {"departure_time", "17:40"},
    };
    UPDATE_FIELD(t, departure_date, updates1);
    UPDATE_FIELD(t, departure_time, updates1);
    UPDATE_FIELD(t, price, updates1);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 0);

    const map<string, string> updates2 = {
            {"price", "12550"},
            {"arrival_time", "20:33"},
    };
    UPDATE_FIELD(t, departure_date, updates2);
    UPDATE_FIELD(t, departure_time, updates2);
    UPDATE_FIELD(t, arrival_time, updates2);
    UPDATE_FIELD(t, price, updates2);

    // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
    // значения этих полей не должны измениться
    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 12550);
    ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestUpdate);
}


//void UpdateTicket(AirlineTicket& ticket, const map<string, string>& updates) {
//    map<string, string>::const_iterator it;
//
//    it = updates.find("to");
//    if (it != updates.end()) {
//        ticket.to = it->second;
//    }
//
//    it = updates.find("from");
//    if (it != updates.end()) {
//        ticket.from = it->second;
//    }
//
//    it = updates.find("price");
//    if (it != updates.end()) {
//        istringstream is(it->second);
//        is >> ticket.price;
//    }
//
//    ...
//}