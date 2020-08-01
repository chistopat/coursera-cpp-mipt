#include <vector>
#include "tests.h"
#include "phone_number.h"

void Test1() {
    try {
        PhoneNumber phoneNumber("+7-985-0005479");
        AssertEqual(phoneNumber.GetCountryCode(), "7", "contryCode");
        AssertEqual(phoneNumber.GetCityCode(), "985", "cityCode");
        AssertEqual(phoneNumber.GetLocalNumber(), "0005479", "localNumber");
        AssertEqual(phoneNumber.GetInternationalNumber(), "+7-985-0005479", "international");

    } catch(std::invalid_argument& e) {
        AssertEqual(e.what(), "", "unexpected");
    }
}

void Test2() {
    try {
        PhoneNumber phoneNumber("+7-985-000-54-79");
        AssertEqual(phoneNumber.GetCountryCode(), "7", "contryCode");
        AssertEqual(phoneNumber.GetCityCode(), "985", "cityCode");
        AssertEqual(phoneNumber.GetLocalNumber(), "000-54-79", "localNumber");
        AssertEqual(phoneNumber.GetInternationalNumber(), "+7-985-000-54-79", "international");

    } catch(std::invalid_argument& e) {
        AssertEqual(e.what(), "", "unexpected");
    }
}

void Test3() {
    try {
        PhoneNumber phoneNumber("+323-22-460002");
        AssertEqual(phoneNumber.GetCountryCode(), "323", "contryCode");
        AssertEqual(phoneNumber.GetCityCode(), "22", "cityCode");
        AssertEqual(phoneNumber.GetLocalNumber(), "460002", "localNumber");
        AssertEqual(phoneNumber.GetInternationalNumber(), "+323-22-460002", "international");

    } catch(std::invalid_argument& e) {
        AssertEqual(e.what(), "", "unexpected");
    }
}

void Test4() {
    try {
        PhoneNumber phoneNumber("+1-2-coursera-cpp");
        AssertEqual(phoneNumber.GetCountryCode(), "1", "contryCode");
        AssertEqual(phoneNumber.GetCityCode(), "2", "cityCode");
        AssertEqual(phoneNumber.GetLocalNumber(), "coursera-cpp", "localNumber");
        AssertEqual(phoneNumber.GetInternationalNumber(), "+1-2-coursera-cpp", "international");

    } catch(std::invalid_argument& e) {
        AssertEqual(e.what(), "", "unexpected");
    }
}

void Test5() {
    try {
        PhoneNumber phoneNumber("1-2-333");
    } catch(std::invalid_argument& e) {
        AssertEqual("Invalid phone", "Invalid phone", "1-2-333");
    }
}

void Test6() {
    try {
        PhoneNumber phoneNumber("+7-1233");
    } catch(std::invalid_argument& e) {
        AssertEqual("Invalid phone", "Invalid phone", "+7-1233");
    }
}

void TestAll() {
  TestRunner tr;
  tr.RunTest(Test1, "1");
    tr.RunTest(Test2, "2");
    tr.RunTest(Test3, "3");
    tr.RunTest(Test4, "4");
    tr.RunTest(Test5, "5");
    tr.RunTest(Test6, "6");

}