#include <iostream>
#include "test_runner.h"
#include "vector"
#include "string"
#include "sstream"
#include "forward_list"
#include "algorithm"
#include <numeric>
#include <vector>
#include <iostream>
#include <iterator>
#include <functional>
#include <memory>

using namespace std;

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
    os << "[";
    bool first = true;
    for (const auto &i : v) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << i;
    }
    os << "]";
    return os;
}
struct NoncopyableInt {
  int value;

  NoncopyableInt(int value) : value(value) {}

  NoncopyableInt(const NoncopyableInt&) = delete;
  NoncopyableInt& operator=(const NoncopyableInt&) = delete;

  NoncopyableInt(NoncopyableInt&&) = default;
  NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator<(const NoncopyableInt& left, const NoncopyableInt& rigth) {
    return left.value < rigth.value;
}

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    //если диапазон содержит только один элемент - выходим
    if(range_end - range_begin < 2) {
        return;
    }
    vector<typename RandomIt::value_type> elements(make_move_iterator(range_begin), make_move_iterator(range_end));
    auto border = distance(range_begin, range_end) / 3;
    auto begin = elements.begin();
    auto m1 = elements.begin() + border;
    auto m2 = elements.begin() + border + border;
    auto end = elements.end();
    MergeSort(make_move_iterator(begin), make_move_iterator(m1));
    MergeSort(make_move_iterator(m1), make_move_iterator(m2));
    MergeSort(make_move_iterator(m2), make_move_iterator(end));
    vector<typename RandomIt::value_type> temp;
    merge(make_move_iterator(begin), make_move_iterator(m1), make_move_iterator(m1), make_move_iterator(m2), back_inserter(temp));
    merge(make_move_iterator(temp.begin()), make_move_iterator(temp.end()), make_move_iterator(m2), make_move_iterator(end), range_begin);
}

void TestIntVector() {
    vector<NoncopyableInt> numbers;
    for (auto i : vector<int>{6, 1, 3, 9, 1, 9, 8, 12, 1}) {
        numbers.emplace_back(i);
    }
    MergeSort(begin(numbers), end(numbers));
    assert(is_sorted(begin(numbers), end(numbers)));
}

int main() {
    TestIntVector();
    return 0;
}
