#include "database.h"

std::ostream& operator<<(std::ostream& os, const DbEntry& entry) {
    os << Date(entry.date[0], entry.date[1], entry.date[2]) << " " << entry.event;
    return os;
}

void Database::Add(const Date &date, const string &event) {
    if(!_UniqueData.empty() and _UniqueData.count(date) > 0) {
        if(_UniqueData.at(date).count(event) > 0) {
            return void();
        }
    }
    _UniqueData[date].insert(event);
    _OrderedData[date].push_back(event);
}

void Database::Print(ostream &os) const {
    for(auto &[date, events] : _OrderedData) {
        for(auto &event : events) {
                os << date << " " << event << std::endl;
            }
        }
    }

DbEntry Database::Last(const Date &lastDate) const {
    auto it = _OrderedData.upper_bound(lastDate);
    if (it == _OrderedData.begin()) {
        throw std::invalid_argument("");
    }
    auto date = (--it)->first;
    auto event = *_OrderedData.at(date).rbegin();
    return DbEntry({date.ToVector(), event});


}

std::list<DbEntry> Database::FindIf(TPred cmp) const {
    std::list<DbEntry> found;
    for(auto &[date, events] : _OrderedData) {
        for(auto &event : events) {
            bool match = cmp(date, event);
            if(match) {
                found.push_back(DbEntry{date.ToVector(), event});
            }
        }
    }
    return found;
}

int Database::RemoveIf(TPred cmp) {
    auto RemoveList = FindIf(cmp);
    int removeCounter = RemoveList.size();
    for (auto &entry : RemoveList) {
        auto currentDate = Date(entry.date);
        auto& eventsFromUniq = _UniqueData.at(currentDate);
        eventsFromUniq.erase(entry.event);
        if(eventsFromUniq.empty()) {
            _UniqueData.erase(currentDate);
        }

        auto& eventsFromOrder = _OrderedData.at(currentDate);
        eventsFromOrder.remove_if([entry](std::string e){return e==entry.event;});
        if(eventsFromOrder.empty()) {
            _OrderedData.erase(currentDate);
        }
    }
    return removeCounter;
}


