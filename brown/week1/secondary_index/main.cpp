#include "test_runner.h"

#include <iostream>
#include <map>
#include <unordered_map>

using namespace std;

struct Record {
    std::string id;
    std::string title;
    std::string user;
    int timestamp;
    int karma;
};

class Database {
public:
    bool Put(const Record& record) {
        bool result = false;
        if (!data_.count(record.id)) {
            DataItem item = {record, index_by_user_.end(), index_by_timestamp_.end(), index_by_karma_.end()};
            data_.insert({record.id, move(item)});
            data_[record.id].index_by_user_iterator = index_by_user_.insert({record.user, &data_[record.id].record});
            data_[record.id].index_by_karma_iterator = index_by_karma_.insert({record.karma, &data_[record.id].record});
            data_[record.id].index_by_timestamp_iterator =
                index_by_timestamp_.insert({record.timestamp, &data_[record.id].record});

            result = true;
        }
        return result;
    }

    [[nodiscard]] const Record* GetById(const std::string& id) const {
        if (data_.count(id)) {
            return &data_.at(id).record;
        }
        return nullptr;
    }

    bool Erase(const std::string& id) {
        bool result = false;
        if (data_.count(id)) {
            auto item_node = data_.extract(id);
            index_by_user_.erase(item_node.mapped().index_by_user_iterator);
            index_by_timestamp_.erase(item_node.mapped().index_by_timestamp_iterator);
            index_by_karma_.erase(item_node.mapped().index_by_karma_iterator);
            result = true;
        }
        return result;
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        auto begin = index_by_timestamp_.lower_bound(low);
        auto end = index_by_timestamp_.upper_bound(high);
        for (auto it = begin; it != end; ++it) {
            if (!callback(*it->second)) {
                break;
            }
        }
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        auto begin = index_by_karma_.lower_bound(low);
        auto end = index_by_karma_.upper_bound(high);
        for (auto it = begin; it != end; ++it) {
            if (!callback(*it->second)) {
                break;
            }
        }
    }

    template <typename Callback>
    void AllByUser(const std::string& user, Callback callback) const {
        auto [begin, end] = index_by_user_.equal_range(user);
        for (auto it = begin; it != end; ++it) {
            if (!callback(*it->second)) {
                break;
            }
        }
    }

private:
    using RecordPtr = const Record*;

private:
    struct DataItem {
        Record record;
        std::multimap<std::string, RecordPtr>::iterator index_by_user_iterator;
        std::multimap<int, RecordPtr>::iterator index_by_timestamp_iterator;
        std::multimap<int, RecordPtr>::iterator index_by_karma_iterator;
    };

private:
    std::unordered_map<std::string, DataItem> data_;
    std::multimap<int, RecordPtr> index_by_timestamp_;
    std::multimap<int, RecordPtr> index_by_karma_;
    std::multimap<std::string, RecordPtr> index_by_user_;
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

    ASSERT_EQUAL(2, count)
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
        ASSERT_EQUAL(5, count)
    }
    {
        int count = 0;
        db.RangeByTimestamp(1536107261, 1536107263, [&count](const Record&) {
            ++count;
            return true;
        });
        ASSERT_EQUAL(3, count)
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

    ASSERT_EQUAL(2, count)
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr)
    ASSERT_EQUAL(final_body, record->title)
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    RUN_TEST(tr, TestRangeTimestampBoundaries);
    return 0;
}
