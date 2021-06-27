#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "test_runner.h"


using namespace std;
using Domain = vector<string>;

Domain ParseDomain(const string& line) {
    vector<string> result;
    stringstream is(line);
    string token;

    while(getline(is, token, '.')) {
        result.push_back(token);
    }
    reverse(result.begin(), result.end());
    return result;
}

void ReadDomains(istream& input, vector<Domain>& result) {
    size_t count;
    input >> count;

    result.resize(count);
    for (auto& item : result) {
        string line;
        input >> line;
        item = ParseDomain(line);
    }
}

ostream& operator<< (ostream& os, const Domain& domain) {
    bool first = true;
    for (const auto& token: domain) {
        if (!first) {
            os << "."s;
        }
        first = false;
        os << token;
    }
    return os;
}

ostream& PrintVector(vector<Domain> v, ostream& os) {
    for (const auto& domain : v) {
        os << domain << endl;
    }
    return os;
}

bool CompareDomains(const Domain& domain, const Domain& sub) {
    if (domain.size() > sub.size()) {
        return false;
    }

    for (size_t i = 0; i < min(domain.size(), sub.size()); ++i) {
        if (domain[i] != sub[i]) {
            return false;
        }
    }
    return true;
}

bool CheckDomainInBan(const Domain& domain, const set<Domain>& banned) {
    auto it = banned.upper_bound(domain);
    return it != begin(banned) && CompareDomains(*prev(it), domain);
}

set<Domain> ShrinkDomains(const vector<Domain>& domains) {
    set<Domain> result;
    for (const auto& domain : domains) {
        if (!CheckDomainInBan(domain, result)) {
            result.insert(domain);
        }
    }
    auto domain_it = result.begin();
    auto sub_domain_it = result.begin();
    ++sub_domain_it;

    while (sub_domain_it != result.end()) {
        if (CompareDomains(*domain_it, *sub_domain_it)) {
            result.erase(sub_domain_it);
        } else {
            domain_it = sub_domain_it;
        }
        sub_domain_it = domain_it;
        ++sub_domain_it;
    }

    return result;
}

void TestCompare() {
    ASSERT(CompareDomains({"ru"}, {"ru", "ya"}))
    ASSERT(!CompareDomains({"ya"}, {"ru", "ya"}))
    ASSERT(CompareDomains({"ru", "ya"}, {"ru", "ya", "mail"}))
    ASSERT(!CompareDomains({"ru", "ya", "mail"}, {"ru", "ya"}))
}

void TestShrink() {
    {
        vector<Domain> input = {{"c", "b"}, {"c", "b", "a"}, {"a"}};
        set<Domain> expected = {{"a"}, {"c", "b"}};

        ASSERT_EQUAL(expected, ShrinkDomains(input))
    }

    {
        vector<Domain> input = {{"c", "b"}, {"c", "b", "a"}, {"c"}};
        set<Domain> expected = {{"c"}};

        ASSERT_EQUAL(expected, ShrinkDomains(input))
    }
}
void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestCompare);
    RUN_TEST(tr, TestShrink);
}

int main() {
    TestAll();
    vector<Domain> domains;
    vector<Domain> forbidden;
    ReadDomains(cin, forbidden);
    ReadDomains(cin, domains);
    auto blacklist = ShrinkDomains(forbidden);
    for (const auto& domain : domains) {
        bool res = CheckDomainInBan(domain, blacklist);
        if (!res) {
            std::cout << "Good" << endl;
            continue;
        }
        std::cout << "Bad" << endl;
    }
}