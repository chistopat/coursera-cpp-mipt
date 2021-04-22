#include "test_runner.h"
#include "profile.h"
#include <set>
#include <utility>
#include <string>

using namespace std;


using pair_type = std::pair<std::string, int>;

auto comp = [](const pair_type& a, const pair_type& b) {
  return a.second <= b.second;
};

struct QueueItem {
  int priority;
  int id;
  std::string value;
};

auto CompareItems = [](const QueueItem& left, const QueueItem& right) {
  return std::make_pair(left.priority, left.id)
         < make_pair(right.priority, right.id);
};

bool operator< (const QueueItem& left, const QueueItem& right) {
  return std::make_pair(left.priority, left.id)
      < make_pair(right.priority, right.id);
}

template <typename T>
void PrintQueue(const T& queue) {
  for (auto& item : queue) {
    std::cout << item.first << ": " << item.second << std::endl;
  }
}


template <typename T>
class PriorityCollection {
public:
  using Id = int;
  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    auto it = queue_.insert({default_priority_, ++serial_, move(object)});
    index_[serial_] = it.first;
    return serial_;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
      for (auto it = range_begin; it != range_end; it++) {
        *ids_begin++ = Add(move(*it));
      }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
    return index_.count(id) == 1;
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
    return index_.at(id)->value;
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    auto temp = move(queue_.extract(index_.at(id)).value());
    temp.priority++;
    index_[id] = queue_.insert(move(temp)).first;
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
    return {queue_.begin()->value, queue_.begin()->priority};
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    index_.erase(queue_.begin()->id);
    auto temp = move(queue_.extract(queue_.begin()).value());
    return {move(temp.value), temp.priority};
  }
  struct Item_ {
    int priority;
    int id;
    T value;
  };

private:

  friend bool operator< (const Item_& left, const Item_& right) {
    return std::make_pair(left.priority, left.id)
        > make_pair(right.priority, right.id);
  }

  std::set<Item_> queue_;
  std::map<Id, typename std::set<Item_>::iterator> index_;
  int serial_ = -1;
  int default_priority_ = 0;
};

class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");


  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
  ASSERT(!strings.IsValid(yellow_id));
  auto asdf = strings.Add("asdf");
  ASSERT_EQUAL(strings.GetMax().first, "asdf");
  ASSERT_EQUAL(strings.Get(asdf), "asdf");
  vector<int> ids;
  vector<string> s = {"a", "b", "c"};
  PriorityCollection<string> pq;
  pq.Add(s.begin(), s.end(), back_inserter(ids));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
