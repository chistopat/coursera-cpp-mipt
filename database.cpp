#include "database.h"

std::ostream& operator<<(std::ostream& os, const DbEntry& entry) {
    os << Date(entry.date[0], entry.date[1], entry.date[2]) << " " << entry.event;
    return os;
}

void Database::Add(const Date &date, const string &event) {
    _db[date].insert(Event({event, ++_entryOrderCounter}));
}

void Database::Print(ostream &os) const {
    std::set<std::string> tempUnique;
    for(auto &[date, events] : _db) {
        for(auto &event : events) {
            if(tempUnique.count(event.event) == 0) {
                os << date << " " << event.event << std::endl;
                tempUnique.insert(event.event);
            }
        }
    }
}


DbEntry Database::Last(const Date &lastDate) const {
    return DbEntry({{1,1,1}, ""});
}

bool operator<(Event lhs, Event rhs) {
    return std::tie(lhs.order, lhs.event)
            < std::tie(rhs.order, rhs.event);
}

