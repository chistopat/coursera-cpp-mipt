#include <iostream>
#include <vector>
#include <algorithm>
#include "test_runner.h"

void PrintPermutaion(int number) {
    std::vector<int> digits(number);
    for (int i=1; i<=number; i++) {
        digits[i-1]=i;
    }
    std::vector<std::vector<int>> results;
    bool next = true;
    while(next){
        results.push_back(digits);
        next = std::next_permutation(digits.begin(), digits.end());
    }
    std::sort(results.rbegin(), results.rend());
    for(auto& r : results) {
        for (auto &d : r) {
            std::cout << d << ' ';
        }
        std::cout << std::endl;
    }
}

int main() {
    int n = 0;
    std::cin >> n;
    PrintPermutaion(n);
    return 0;
}
