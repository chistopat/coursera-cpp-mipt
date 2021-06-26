//
// Created by chistopat on 26.06.2021.
//
#include <iostream>

#include "tests.h"
#include "components.h"


int main() {
    TestAll();
    const auto requests = ReadRequests(std::cin);
    return 0;
}