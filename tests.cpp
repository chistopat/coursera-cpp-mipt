#include "tests.h"

void TestAll() {
  TestRunner tr;
  tr.RunTest([]() { }, "lambda");
}