#include "test_runner.h"
#include "point.h"
#include "stop.h"
#include "bus_route.h"
#include "transport_manager.h"
#include "request.h"
#include "utils.h"
#include "tests.h"

#include <iostream>
#include <memory>

using namespace std;

void TestAll() {
    TestRunner test_runner;
    RUN_TEST(test_runner, Tests::AddBus);
    RUN_TEST(test_runner, Tests::AddStop);
    RUN_TEST(test_runner, Tests::ParseStopRequest);
    RUN_TEST(test_runner, Tests::Smoke);
    RUN_TEST(test_runner, Tests::AddBusSameStops);
    RUN_TEST(test_runner, Tests::LengthBetweenSamePoints);
//    RUN_TEST(test_runner, Tests::AddStopSpaces);
    RUN_TEST(test_runner, Tests::CheckStrip);
    RUN_TEST(test_runner, Tests::FuzzNames);
    RUN_TEST(test_runner, Tests::TestEmptyBase);
}


int main() {
    const bool DEBUG = false;
//    TestAll();
    cout.precision(6);
    TransportManager manager;
    ostringstream log;
    const auto requests_to_write = ReadRequests(DataBaseMode::WRITE, log, cin);
    const auto requests_to_read = ReadRequests(DataBaseMode::READ, log, cin, DEBUG, "vW1R0hGBlnaSNo6M58N3");
    ProcessWriteRequests(requests_to_write, manager);
    const auto responses = ProcessReadRequests(requests_to_read, manager);
    PrintResponses(responses, cout);

    return 0;
}