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

using namespace std;


std::ostream& operator<<(std::ostream& os, std::vector<int> v) {
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

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    //если диапазон содержит только один элемент - выходим
    if(range_end - range_begin < 2) {
        return;
    }
    vector<typename RandomIt::value_type> elements(range_begin, range_end);
    auto border = distance(range_begin, range_end) / 2;
    MergeSort(elements.begin(), elements.begin()+border);
    MergeSort(elements.begin()+border, elements.end());
    merge(elements.begin(), elements.begin()+border, elements.begin()+border, elements.end(), range_begin);
}



int main() {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}