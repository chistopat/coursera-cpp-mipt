#include "tests.h"
// Не меняя тела функции main, реализуйте функции и классы выше
//template <typename T> void PrintVector(const vector<T> &container) {
//    cout << "[";
//    bool first = true;
//    for (const auto &i : container) {
//        if (!first) {
//            cout << ", ";
//        }
//        first = false;
//        cout << i;
//    }
//    cout << "]";
//}
//
//void TestAddBus() {
//    BusManager bm;
//    Query bus32{
//            QueryType::NewBus, "32", "", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}};
//    Query bus32k{QueryType::NewBus,
//                 "32K",
//                 "",
//                 {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino",
//                  "Solntsevo", "Skolkovo"}};
//    Query bus950{QueryType::NewBus,
//                 "950",
//                 "",
//                 {"Kokoshkino", "Marushkino", "Vnukovo", "Peredelkino",
//                  "Solntsevo", "Troparyovo"}};
//    Query bus272{QueryType::NewBus,
//                 "272",
//                 "",
//                 {"Vnukovo", "Moskovsky", "Rumyantsevo", "Troparyovo"}};
//
//    bm.AddBus(bus32.bus, bus32.stops);
//    bm.AddBus(bus32k.bus, bus32k.stops);
//    auto busToStopsMap = bm.GetBusToStopsMap();
//    auto stopToBusesMap = bm.GetStopToBusesMap();
//    vector<string> expectedBuses = {"32", "32K"};
//    vector<string> expectedBus = {"32K"};
//
//    vector<string> expectedBusesTroparevo = {"950", "272"};
//
//    assert(busToStopsMap[bus32.bus] == bus32.stops);
//    assert(busToStopsMap[bus32k.bus] == bus32k.stops);
//    assert(equal(stopToBusesMap["Tolstopaltsevo"].begin(),
//                 stopToBusesMap["Tolstopaltsevo"].end(), expectedBuses.begin()));
//    assert(equal(stopToBusesMap["Marushkino"].begin(),
//                 stopToBusesMap["Marushkino"].end(), expectedBuses.begin()));
//    assert(equal(stopToBusesMap["Skolkovo"].begin(),
//                 stopToBusesMap["Skolkovo"].end(), expectedBus.begin()));
//    bm.AddBus(bus950.bus, bus950.stops);
//    bm.AddBus(bus272.bus, bus272.stops);
//    assert(equal(stopToBusesMap["Troparyovo"].begin(),
//                 stopToBusesMap["Troparyovo"].end(),
//                 expectedBusesTroparevo.begin()));
//    cout << "[PASS] TestAddBuss" << endl;
//}
//
//void TestBusesForStopResponse() {
//    BusManager bm;
//    Query bus32{
//            QueryType::NewBus, "32", "", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}};
//    Query bus32k{QueryType::NewBus,
//                 "32K",
//                 "",
//                 {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino",
//                  "Solntsevo", "Skolkovo"}};
//    // Test for empty bm
//    auto testResponse = bm.GetBusesForStop(bus32.stop);
//    assert(testResponse.stop.empty() && testResponse.buses.empty());
//
//    bm.AddBus(bus32.bus, bus32.stops);
//    bm.AddBus(bus32k.bus, bus32k.stops);
//    vector<string> expectedBuses = {"32", "32K"};
//    vector<string> expectedBus = {"32K"};
//
//    auto marushkino = bm.GetBusesForStop("Marushkino");
//    assert(marushkino.stop == "Marushkino");
//    assert(equal(marushkino.buses.begin(), marushkino.buses.end(),
//                 expectedBuses.begin()));
//
//    auto skolkovo = bm.GetBusesForStop("Skolkovo");
//    assert(skolkovo.stop == "Skolkovo");
//    assert(
//            equal(skolkovo.buses.begin(), skolkovo.buses.end(), expectedBus.begin()));
//
//    cout << "[PASS] TestBusesForStopResponse" << endl;
//}
//
//void TestStopsForBusResponse() {
//    BusManager bm;
//    Query bus32{
//            QueryType::NewBus, "32", "", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}};
//    Query bus32k{QueryType::NewBus,
//                 "32K",
//                 "",
//                 {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino",
//                  "Solntsevo", "Skolkovo"}};
//    Query bus950{QueryType::NewBus,
//                 "950",
//                 "",
//                 {"Kokoshkino", "Marushkino", "Vnukovo", "Peredelkino",
//                  "Solntsevo", "Troparyovo"}};
//    Query bus272{QueryType::NewBus,
//                 "272",
//                 "",
//                 {"Vnukovo", "Moskovsky", "Rumyantsevo", "Troparyovo"}};
//    auto emptyResponce = bm.GetStopsForBus("Solntsevo");
//    assert(emptyResponce.bus.empty() && emptyResponce.interchanges.empty());
//
//    bm.AddBus(bus32.bus, bus32.stops);
//    bm.AddBus(bus32k.bus, bus32k.stops);
//    bm.AddBus(bus950.bus, bus950.stops);
//    bm.AddBus(bus272.bus, bus272.stops);
//
//    vector<pair<string, vector<string>>> expected = {
//            {"Vnukovo", {"32", "32K", "950"}},
//            {"Moskovsky", {}},
//            {"Rumyantsevo", {}},
//            {"Troparyovo", {"950"}}};
//    auto response = bm.GetStopsForBus("272");
//    assert(response.bus == "272");
//    assert(equal(response.interchanges.begin(), response.interchanges.end(),
//                 expected.begin()));
//    cout << "[PASS] TestStopsForBusResponse" << endl;
//}

void TestAll() {
  TestRunner tr;
  tr.RunTest([]() { std::cout << "OMFG" << std::endl; }, "lambda");
}