#pragma once

#include <exception>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;

template <class T> ostream &operator<<(ostream &os, const set<T> &s) {
  os << "{";
  bool first = true;
  for (const auto &x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream &operator<<(ostream &os, const map<K, V> &m) {
  os << "{";
  bool first = true;
  for (const auto &kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template <class T> ostream &operator<<(ostream &os, const vector<T> &s) {
    os << "[";
    bool first = true;
    for (const auto &x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "]";
}

template <class T, class U>
void AssertEqual(const T &t, const U &u, const string &hint) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u << " hint: " << hint;
    throw runtime_error(os.str());
  }
}

template <class T, class U>
void AssertEqual(const vector<T> &t, const vector<U> &u, const string &hint) {
    if (!std::equal(t.begin(), t.end(), u.begin())) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u << " hint: " << hint;
        throw runtime_error(os.str());
    }
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string &test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    } catch (runtime_error &e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    }
  }
  ~TestRunner();

private:
  int fail_count = 0;
};
