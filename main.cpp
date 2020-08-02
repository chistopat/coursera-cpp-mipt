#include <iostream>
#include "algorithm"
#include "vector"

void PrintVectorPart(const std::vector<int> &numbers){
    if (numbers.empty()) {
        return;
    }
    auto firstNegative = std::find_if(numbers.begin(), numbers.end(), [](int x){return x<0;});
    for (auto it=--firstNegative; it>=numbers.begin(); --it){
        std::cout << *it << ' ';
    }
}

int main() {
    PrintVectorPart({6, 1, 8, -5, 4});
    std::cout << std::endl;
    PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
    std::cout << std::endl;
    PrintVectorPart({6, 1, 8, 5, 4});
    std::cout << std::endl;
    PrintVectorPart({});
    std::cout << std::endl;

    return 0;
}
