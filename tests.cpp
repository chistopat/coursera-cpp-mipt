#include "tests.h"

void TestSum() {
    AssertEqual(Sum(2,3), 5, "2+3=5");
}

void TestReverse() {
    AssertEqual(Reverse("abc"), "cba", "abc=cba");
}

void TestSort() {
    std::vector<int> nums = {1, 4, 3};
    Sort(nums);
    std::vector<int> exp = {1, 3, 4};
    AssertEqual(nums, exp, "abc=cba");
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestSum, "TestSum");
    tr.RunTest(TestReverse, "TestReverse");
    tr.RunTest(TestSort, "TestSort");
}