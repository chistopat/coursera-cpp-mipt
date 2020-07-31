#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

bool IsPalindrom(const string& s) {
    return std::equal(s.begin(), s.begin() + s.size()/2, s.rbegin());
}

void TestEmpty() {
    Assert(IsPalindrom(""), "");
}

void TestSpace() {
    Assert(IsPalindrom(" "), " ");
}

void TestOdd() {
    Assert(IsPalindrom("amama"), "amama");
}

void TestEven() {
    Assert(IsPalindrom("amma"), "amma");
}

void TestSpaces() {
    Assert(!IsPalindrom(" amma"), " amma");
    Assert(!IsPalindrom("amma "), "amma ");
    Assert(!IsPalindrom("amma  "), "amma  ");
    Assert(!IsPalindrom(" amma  "), " amma  ");
    Assert(!IsPalindrom("  amma "), "  amma ");
    Assert(IsPalindrom(" amma "), " amma ");
    Assert(IsPalindrom("  amma  "), "  amma  ");
    Assert(IsPalindrom("   amma   "), "   amma   ");


}


void TestEndl() {
    Assert(!IsPalindrom("amma\n"), "amma n");
}

void TestTab() {
    Assert(!IsPalindrom("amma\t"), "amma t");
}

void TestR() {
    Assert(!IsPalindrom("amma\r"), "amma r");
}

void TestLong() {
    Assert(!IsPalindrom("aaaaaaaaaaaaaaazdfaaaaaaaaaaaaaaa"), "aaaaaaaaaaaaaaazdfaaaaaaaaaaaaaaa");
    Assert(!IsPalindrom("baobab"), "baobab");
}

void TestCaps() {
    Assert(IsPalindrom("AMMA"), "AMMA");
    Assert(IsPalindrom("aMMa"), "aMMa");
    Assert(IsPalindrom("AmmA"), "AmmA");
    Assert(!IsPalindrom("aMMA"), "aMMA");
    Assert(!IsPalindrom("AMMa"), "AMMa");
}


void TestSymbols() {
    Assert(!IsPalindrom("±!@#$%^&*()_+{}:'?><,./`~'"), "symbols");
    Assert(IsPalindrom("!@#$%^&*()_+{}:'?><,./`~''~`/.,<>?':}{+_)(*&^%$#@!"), "symbols");
}


int main() {
    TestRunner runner;
    runner.RunTest(TestEmpty, "TestEmpty");
    runner.RunTest(TestSpace, "TestSpace");
    runner.RunTest(TestOdd, "TestOdd");
    runner.RunTest(TestEven, "TestEven");
    runner.RunTest(TestSpaces, "TestSpaceFirst");
    runner.RunTest(TestEndl, "TestEndl");
    runner.RunTest(TestR, "TestR");
    runner.RunTest(TestTab, "TestTab");
    runner.RunTest(TestLong, "TestLong");
    runner.RunTest(TestCaps, "TestCaps");
    runner.RunTest(TestSymbols, "TestSymbols");


    // добавьте сюда свои тесты
    return 0;
}
