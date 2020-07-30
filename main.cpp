#include <iostream>
#include "utility"
#include "map"
#include "vector"
using namespace std;


// Предварительное объявление шаблонных функций
template<typename T> T Sqr(T x);
template <typename R, typename L> pair<R, L> operator* (const pair<R, L>& p1, const pair<R, L>& p2);
template <typename K, typename V>map<K, V> operator* (const map<K, V>& m1, const map<K, V>& m2);
template <typename T> vector<T> operator* (const vector<T>& v1, const vector<T>& v2);

template <typename R, typename L>
pair<R, L> operator* (const pair<R, L>& p1, const pair<R, L>& p2) {
    return {p1.first * p2.first, p1.second * p2.second};
}

template <typename K, typename V>
map<K, V> operator* (const map<K, V>& m1, const map<K, V>& m2) {
    map<K, V> result;
    for (auto& [k,v] : m1) {
        result[k] = v * v;
    }
    return result;
}

template <typename T>
vector<T> operator* (const vector<T>& v1, const vector<T>& v2) {
    vector<T> result;
    for (auto& i : v1) {
        result.push_back(i*i);
    }
    return result;
}

// Объявляем шаблонные функции
template <typename T>
T Sqr(T x) {
    return x * x;
}


int main() {
    Sqr(1);
    Sqr(1.0);
    Sqr('a');
    Sqr(make_pair(1,2));
    Sqr(vector<pair<int, int>>{{1,2}, {3, 4}});
    Sqr(map<int, pair<int, int>>{{1, {3, 4}}});
    Sqr(vector<map<int, pair<int, int>>>{{{1, {3, 4}}}, {{1, {3, 4}}}});
    return 0;
}
