#include <iostream>
#include "algorithm"
#include "vector"
#include "set"


template <typename T>
std::vector<T>
FindGreaterElements(const std::set<T>& elements, const T& border) {
    return {elements.upper_bound(border), elements.end()};
}


int main() {
    FindGreaterElements(std::set<int>{1, 5, 7, 8}, 5);
    for (int x : FindGreaterElements(std::set<int>{1, 5, 7, 8}, 5)) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::string to_find = "Python";
    std::cout << FindGreaterElements(std::set<std::string>{"C", "C++"}, to_find).size() << std::endl;
    return 0;
}
