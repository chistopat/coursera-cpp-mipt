#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <deque>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate() {
    if(deallocated.empty()) {
      T* object = new T;
      allocated.insert(object);
      return object;
    }
    return _Allocate();
  }
  T* TryAllocate() {
    if (deallocated.empty()) {
      return nullptr;
    }
    return _Allocate();
  }

  void Deallocate(T* object) {
    if (allocated.count(object) == 0) {
      throw invalid_argument("");
    }
    allocated.erase(object);
    deallocated.push_back(object);
  }

  ~ObjectPool() {
    for (auto& ptr : deallocated) {
      delete ptr;
    }
    for (auto& ptr : allocated) {
      delete ptr;
    }
  }

private:
  set<T*> allocated;
  deque<T*> deallocated;

  T* _Allocate() {
    auto item = deallocated.front();
    allocated.insert(item);
    deallocated.pop_front();
    return item;
  }
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
