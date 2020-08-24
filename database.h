#pragma once
#include "date.h"
#include "functional"
#include "iostream"
#include "vector"
#include "map"
#include "set"
#include "list"
#include "stdexcept"

typedef std::function<bool(const Date& date,
                           const string& event)> TPred;

struct DbEntry {
    vector<int> date;
    std::string event;
};

class Database {
public:
    void Add(const Date& date, const std::string& event);
    void Print(std::ostream& os) const;
    int RemoveIf(TPred cmp);
    std::list<DbEntry> FindIf(TPred cmp) const;
    DbEntry Last(const Date& lastDate) const;

private:
    map<Date, std::set<std::string>> _UniqueData;
    map<Date, std::list<std::string>> _OrderedData;
};

std::ostream& operator<<(std::ostream& os, const DbEntry& entry);