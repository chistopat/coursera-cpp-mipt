#include <iostream>
#include "cinttypes"
#include "cassert"
#include "vector"
#include "sstream"
#include "string"

using namespace std;


uint64_t CalculateBlocksMass(const uint64_t nBlocks, const uint64_t density) {
    uint64_t amount = 0;
    for (uint64_t i = 0; i < nBlocks; i++) {
        uint64_t height, width, depth;
        cin >> width >> height >> depth;
        uint64_t weigth = width * height * depth * density;
        amount += weigth;
    }
    return amount;
}

int main() {
    uint64_t nBlocks = 0;
    uint64_t density = 0;
    cin >> nBlocks >> density;
    auto amount = CalculateBlocksMass(nBlocks, density);
    cout << amount;
    return 0;
}


