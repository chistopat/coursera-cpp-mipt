#include "test_runner.h"
#include "point.h"
#include "stop.h"
#include "bus_route.h"
#include "transport_manager.h"
#include "request.h"
#include "utils.h"
#include "tests.h"
#include "debug.h"

#include <iostream>
#include <memory>

using namespace std;

void TestAll() {
    TestRunner test_runner;
   RUN_TEST(test_runner, Tests::Test3);
    RUN_TEST(test_runner, Tests::JsonSmoke);
}


int main() {
//    TestAll();
    TransportManager manager;
    ostringstream log;
    const auto requests = ReadRequests(cin, DEBUG, TOKEN);
    const auto responses = ProcessRequests(requests, manager);
    PrintResponses(responses, cout);
    return 0;
}