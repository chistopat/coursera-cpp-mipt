#include <iomanip>
#include <iostream>
#include <vector>
#include "set"
#include "map"
#include "numeric"
#include "profile.h"
#include "test_runner.h"
#include "algorithm"
#include "numeric"

using namespace std;

typedef int TPageNumber, TUserId;

struct Reader {
  TUserId userId;
  TPageNumber page_count;
};

bool operator<(const Reader& lhs, const Reader& rhs) {
  if (lhs.page_count == rhs.page_count) {
    return lhs.userId < rhs.userId;
  }
  return lhs.page_count < rhs.page_count;
}

//class ReadingManager {
//public:
//  ReadingManager() = default;
//  void Read(int user_id, int page_count) {
//    if (_users.count(user_id) == 1) {
//      _pages_to_readers[_users[user_id]].erase(user_id);
//    }
//    _pages_to_readers[page_count].insert(user_id);
//    _users[user_id] = page_count;
//  }
//  double Cheer(int user_id) const {
//    if (_users.count(user_id) == 0) {
//      return 0;
//    }
//    int total = _users.size();
//    if (total == 1) {
//      return 1;
//    }
//    int count = 0;
//    const int& current_user_page = _users.at(user_id);
//
//    for(int i = 0; i < current_user_page; ++i) {
//      if (_pages_to_readers.count(i) == 1) {
//        count += _pages_to_readers.at(i).size();
//      }
//    }
//    return  (count * 1.0) / --total;
//  }
//private:
//  map<TPageNumber, set<TUserId>> _pages_to_readers;
//  map<TUserId, TPageNumber> _users;
//};

class ReadingManager {
public:
  ReadingManager() : rating(1000, 0) {}
  void Read(int user_id, int page_count) {
    if(users.count(user_id) == 1) {
      rating[users[user_id]]--;
    }
    users[user_id] = page_count;
    rating[page_count]++;
  }
  double Cheer(int user_id) const {
    if (users.count(user_id) == 0) {
      return 0;
    }
    int total = users.size();
    if (total == 1) {
      return 1;
    }
    const auto& current_page = users.at(user_id);
    int users_count = 0;
    for (int i = 0; i < current_page; ++i) {
      users_count += rating[i];
    }
    return users_count * 1.0 / --total;
  }
private:
  vector<int> rating;
  std::map<TUserId , TPageNumber> users;
};


void TestRead() {
  LogDuration ld("TestRead");
  int MAX = 1'000'000;
  vector<int> users(MAX);
  vector<int> pages(MAX);

  std::iota(users.begin(), users.end(), 1);
  for(int i = 0; i < 1000; i++) {
    auto f = users.begin();
    iota(f, f+1000, 1);
  }
  ReadingManager rm;
  for(int i = 0; i < MAX; i++) {
    rm.Read(users[i], pages[i]);
  }
}

void TestCheer() {
  LogDuration ld("TestCheer");
  ReadingManager rm;
  ASSERT_EQUAL(rm.Cheer(5), 0);
  rm.Read(1, 10);
  ASSERT_EQUAL(rm.Cheer(1), 1);
  rm.Read(2, 5);
  rm.Read(3, 7);
  ASSERT_EQUAL(rm.Cheer(2), 0);
  ASSERT_EQUAL(rm.Cheer(3), 0.5);
  rm.Read(3, 10);
  ASSERT_EQUAL(rm.Cheer(3), 0.5);
}

void TestSamePages() {
  ReadingManager rm;
  rm.Read(1, 1);
  rm.Read(2, 2);
  rm.Read(3, 2);
  rm.Read(4, 2);
  rm.Read(5, 2);
  ASSERT_EQUAL(rm.Cheer(2), (1.0/4));
  rm.Read(1, 2);
  ASSERT_EQUAL(rm.Cheer(2), 0);
  ASSERT_EQUAL(rm.Cheer(1), 0);
}

void TestOneCheer() {
  ReadingManager rm;
  rm.Read(125, 1);
  ASSERT_EQUAL(rm.Cheer(999), 0);
}

int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  TestRunner tr;
  RUN_TEST(tr, TestCheer);
  RUN_TEST(tr, TestSamePages);
  RUN_TEST(tr, TestOneCheer);

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}