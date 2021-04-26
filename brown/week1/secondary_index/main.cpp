#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;

  friend bool operator< (const Record& left, const Record& right) {
      return left.id < right.id;
  }
};


//class Database {
//public:
//  using RecordsIterator = Record*;
//public:
//  bool Put(const Record& record) {
//      if (!id_index_.count(record.id)) {
//
//
//          id_index_.insert({record.id, *record});
//          user_index_.insert({record.user, &record});
//          timestamp_index_.insert({record.timestamp, it});
//          karma_index_.insert({record.karma, it});
//      }
//      return is_inserted;
//  }
//
//  const Record* GetById(const string& id) const {
//      if (id_index_.count(id)) {
//          return &(*id_index_.at(id));
//      }
//      return nullptr;
//  }
//
//  bool Erase(const string& id) {
//
//  }
//
//  template <typename Callback>
//  void RangeByTimestamp(int low, int high, Callback callback) const {
//      auto begin = timestamp_index_.lower_bound(low);
//      auto end = timestamp_index_.upper_bound(high);
//      CallbackIterate(begin, end, callback);
//  }
//
//  template <typename Callback>
//  void RangeByKarma(int low, int high, Callback callback) const {
//      auto begin = karma_index_.lower_bound(low);
//      auto end = karma_index_.upper_bound(high);
//      CallbackIterate(begin, end, callback);
//  }
//
//  template <typename Callback>
//  void AllByUser(const string& user, Callback callback) const {
//      auto [begin, end] = user_index_.equal_range(user);
//      CallbackIterate(begin, end, callback);
//  }
//
//private:
//  struct DBItem{
//      Record* record_ptr;
//      std::unordered_map<std::string, Record*>::iterator id_it;
//      std::multimap<std::string, Record*>::iterator user_it;
//      std::multimap<int, Record*>::iterator timestamp_it;
//      std::multimap<int, Record*>::iterator karma_it;
//
//      friend bool operator< (const DBItem& left, const DBItem& right) {
//          return left.record_ptr->id < right.record_ptr->id;
//      }
//  };
//
//  std::set<DBItem> records_;
//  std::unordered_map<std::string, RecordsIterator> id_index_;
//  std::multimap<std::string, RecordsIterator> user_index_;
//  std::multimap<int, RecordsIterator> timestamp_index_;
//  std::multimap<int, RecordsIterator> karma_index_;
//
//  template<class Callback, class Iterator>
//  static void CallbackIterate(Iterator begin, Iterator end, Callback callback) {
//      for (auto it = begin; it!=end; ++it) {
//          if(!callback(*it->second)) {
//              break;
//          }
//      }
//  }
//};

class Database {
public:
    struct Index {

    };
    using DatabaseItem = std::pair<Record, Index>;
public:

  bool Put(const Record& record) {}

  const Record* GetById(const string& id) const {}

  bool Erase(const string& id) {}

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {}

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {}

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {}

private:


};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});



    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestRangeTimestampBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107261, bad_karma});
    db.Put({"id3", "O>>-<", "general2", 1536107262, bad_karma});
    db.Put({"id4", "O>>-<", "general2", 1536107263, bad_karma});
    db.Put({"id5", "O>>-<", "general2", 1536107264, bad_karma});


    {
        int count = 0;
        db.RangeByTimestamp(1536107259, 1536107265, [&count](const Record&) {
            ++count;
            return true;
        });
        ASSERT_EQUAL(5, count);
    }
    {
        int count = 0;
        db.RangeByTimestamp(1536107261, 1536107263, [&count](const Record&) {
            ++count;
            return true;
        });
        ASSERT_EQUAL(3, count);
    }


}



void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

void TestId() {
    Database db;
    std::vector<const std::string> ids{"a", "b", "c", "d"};
    for (const auto& i : ids) {
        db.Put({i});
        ASSERT_EQUAL(i, db.GetById(i)->id);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    RUN_TEST(tr, TestRangeTimestampBoundaries);
    RUN_TEST(tr, TestId);
    return 0;
}
