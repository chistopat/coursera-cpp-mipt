#include <iostream>
#include "test_runner.h"
#include "vector"
#include "algorithm"

using namespace std;

void PrintVectorPart(const vector<int>& numbers);

void PrintVectorPart(const vector<int>& numbers){
    auto firstNegativeIt = find_if(numbers.begin(), numbers.end(), [](int n){return n<0;});
    if(firstNegativeIt!=numbers.begin()) {
        for(auto it = --firstNegativeIt; it>=numbers.begin(); --it) {
            cout << *it << ' ';
        }
    }
}

int main() {
    PrintVectorPart({6, 1, 8, -5, 4});
    cout << endl;
    PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
    cout << endl;
    PrintVectorPart({6, 1, 8, 5, 4});
    cout << endl;
    PrintVectorPart({});
    cout << endl;
    return 0;
    return 0;
}
