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



bool f(int i) {
    return i == 5;
}

void PrintVector(vector<int>& v) {
    for (auto& i : v) {
        cout << i << ' ';
    }
    cout << endl;
}

int main() {
    {
        //works
        set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
        vector<int> v;
        remove_copy_if(begin(s), end(s), back_inserter(v), f);
        cout << "remove_copy_if(s, f): ";
        PrintVector(v);
    }
    {
        //fail
        cout << "remove_if(s) : ";
        cout << "fail" <<endl;
//        try {
//            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
//            vector<int> v;
//            auto it = remove_if(begin(s), end(s),
//                                [](int x) { return !f(x); });
//            copy(it, end(s), back_inserter(v));
//            PrintVector(v);
//        } catch (...) {
//            cout << "remove_if does not work for set";
//        }

    }
    {
        //почему это работает?
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
            v.assign(begin(s), end(s));
            sort(begin(v), end(v),
                 [](int lhs, int rhs) { return f(lhs) > f(rhs); });
            auto it = partition_point(begin(v), end(v), f);
            v.erase(begin(v), it);
            cout << "v.assign(s), sort(v), partition_point, erase : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }

    }
    {
        //почему это работает?
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
            v.assign(begin(s), end(s));
            auto it = remove_if(begin(v), end(v),
                                [](int x) { return !f(x); });
            v.erase(it, end(v));
            cout << "v.assign(s), remove_if(v, !f), erase(v) : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }

    }
    {
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
            v.assign(begin(s), end(s));
            auto it = partition(begin(v), end(v), f);
            v.erase(it, end(v));
            cout << "v.assign(s), partition(v, f), erase(v) : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }
    }
    {
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
            v.assign(begin(s), end(s));
            sort(begin(v), end(v),
                 [](int lhs, int rhs) { return f(lhs) > f(rhs); });
            auto it = partition_point(begin(v), end(v), f);
            v.erase(it, end(v));
            cout << "v.assign(s), sort(v, f), partition_point(v, f), erase(f) : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }
    }

    {
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
            remove_copy_if(begin(s), end(s), back_inserter(v),
                           [](int x) { return !f(x); });
            cout << "remove_copy_if(v, !f) : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }
    }

    {
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;

            v.assign(begin(s), end(s));
            auto it = remove_if(begin(v), end(v), f);
            v.erase(it, end(v));

            cout << "assign(s) remove_if(v, f) : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }
    }

    {
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
            vector<int> garbage;
            partition_copy(
                    begin(s), end(s),
                    back_inserter(garbage), back_inserter(v), f);
            cout << "partition_copy(v, f) : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }
    }

    {
        //не работает partition для set

        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
//            auto it = partition(begin(s), end(s), f);
//            copy(it, end(s), back_inserter(v));
            cout << "partition(s, f) copy(s, v): ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }
    }

    {
        //не работает partition для set
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
            vector<int> garbage;
            partition_copy(
                    begin(s), end(s),
                    back_inserter(v), back_inserter(garbage), f);
            cout << "partition_copy(s, v, g, f) : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }
    }

    {
        //не работает partition для set
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
            v.assign(begin(s), end(s));
            auto it = partition(begin(v), end(v), f);
            v.erase(begin(v), it);
            cout << "partition(v, f), erase(begin) : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }
    }

    {
        //не работает partition для set
        try {
            set<int> s = { 1, 2, 3, 1, 2, 3, 4, 5, 6};
            vector<int> v;
            copy_if(begin(s), end(s), back_inserter(v), f);
            cout << "copy_if(v, f) : ";
            PrintVector(v);
        } catch (...) {
            cout << "remove_if does not work for set";
        }
    }
    return 0;
}


