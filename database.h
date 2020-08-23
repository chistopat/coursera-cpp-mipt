#pragma once
#include "date.h"
#include "functional"
#include "iostream"
#include "vector"
#include "map"
#include "set"

struct Event {
    std::string event;
    size_t order;
};


struct DbEntry {
    vector<int> date;
    std::string event;
};

class Database {
public:
    void Add(const Date& date, const std::string& event);
    void Print(std::ostream& os) const;

    template<typename TPred>
    int RemoveIf(TPred cmp) {
        size_t rmCount = 0u;
        for(auto &[date, events] : _db) {
            for(auto &event : events) {
                if(cmp(date, event)){
                    ++rmCount;
                    events.erase(event);
                    if (events.empty()) {
                        _db.erase(date);
                    }
                }
            }
        }
        return 0;
    }
    template<typename TPred>
    std::vector<DbEntry> FindIf(TPred cmp) const {

    };
    DbEntry Last(const Date& lastDate) const;

private:
    size_t _entryOrderCounter = 0u;
    map<Date, std::set<Event>> _db;
};

std::ostream& operator<<(std::ostream& os, const DbEntry& entry);
bool operator<(Event lhs, Event rhs);