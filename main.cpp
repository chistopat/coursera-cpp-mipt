#include <string>
#include <vector>
#include "deque"
#include "test_runner.h"
#include "deque_test.h"
#include "deque.h"

using namespace std;

//void Test() {
//  {
//    Deque<int> d;
//    d.Back();
//    d.Front();
//    d.Size();
//  }
//
//  {
//    Deque<int> d;
//    d.PushBack(1);
//    d.PushBack(2);
//    d.PushBack(3);
//    d.PushFront(5);
//    d.PushFront(7);
//    d.PushFront(6);
//    auto f = d.At(0);
//    auto s = d.At(3);
//    ASSERT_EQUAL(f, 6);
//    ASSERT_EQUAL(s, 1);
//  }
//
//  {
//    Deque<int> d;
//    d.PushBack(1);
//    d.PushBack(2);
//    d.PushBack(3);
//    d.Back() = 4;
//    ASSERT_EQUAL(d.Back(), 4);
//  }
//
//  {
//    Deque<int> d;
//    d.PushBack(1);
//    d.PushBack(2);
//    d.PushBack(3);
//    ASSERT_EQUAL(d.Back(), 3);
//  }
//
//  {
//    Deque<int> d;
//    ASSERT_EQUAL(d.Empty(), true);
//    d.PushBack(1);
//    ASSERT_EQUAL(d.Empty(), false);
//    d.PushFront(2);
//    ASSERT_EQUAL(d.Empty(), false);
//  }
//
//}


int main() {
  TestRunner tr;
  Deque<int> d;
  d.PushBack(1);
  d.PushBack(2);
  d.PushBack(3);
  d.PushBack(4);

  cout << d.Front();
  cout << d[0];
  cout << d.At(0);

//  RUN_TEST(tr, Test);
}


