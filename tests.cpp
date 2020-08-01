#include "tests.h"

void TestAll() {
    TestRunner tr;
    tr.RunTest([](){cout << "OMFG" << endl;}, "lambda");
}