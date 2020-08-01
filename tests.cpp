#include "tests.h"

void TestAll() {
  TestRunner tr;
  tr.RunTest([]() { std::cout << "OMFG" << std::endl; }, "lambda");
}