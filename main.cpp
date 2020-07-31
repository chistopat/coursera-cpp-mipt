#include "utility"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class QueryType { NewBus, BusesForStop, StopsForBus, AllBuses };

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

istream &operator>>(istream &is, Query &q) {
  const map<string, QueryType> QueryTypeMap = {
      {"NEW_BUS", QueryType::NewBus},
      {"BUSES_FOR_STOP", QueryType::BusesForStop},
      {"STOPS_FOR_BUS", QueryType::StopsForBus},
      {"ALL_BUSES", QueryType::AllBuses},
  };
  string queryTypeString;
  is >> queryTypeString;
  q.type = QueryTypeMap.at(queryTypeString);
  switch (q.type) {
  case QueryType::NewBus: {
    is >> q.bus;
    size_t stopsCount;
    is >> stopsCount;
    q.stops.resize(stopsCount);
    for (auto &s : q.stops) {
      is >> s;
    }
    break;
  }
  case QueryType::BusesForStop:
    is >> q.stop;
    break;
  case QueryType::StopsForBus:
    is >> q.bus;
    break;
  case QueryType::AllBuses:
    break;
  }
  return is;
}

struct BusesForStopResponse {
  string stop;
  vector<string> buses;
};

ostream &operator<<(ostream &os, const BusesForStopResponse &r) {
  if (r.stop.empty()) {
    os << "No stop";
  } else {
    bool first = true;
    for (auto &b : r.buses) {
      if (!first) {
        os << ' ';
      }
      first = false;
      os << b;
    }
  }
  return os;
}

struct StopsForBusResponse {
  string bus;
  vector<pair<string, vector<string>>> interchanges;
};

ostream &operator<<(ostream &os, const StopsForBusResponse &r) {
  // Реализуйте эту функцию
  if (r.bus.empty()) {
    os << "No bus";
  } else {
    bool firstStop = true;
    for (const auto &[stop, buses] : r.interchanges) {
      if (!firstStop) {
        cout << endl;
      }
      firstStop = false;
      os << "Stop " << stop << ":";
      if (buses.empty()) {
        os << " no interchange";
      } else {
        for (const auto &b : buses) {
          os << ' ' << b;
        }
      }
    }
  }
  return os;
}

struct AllBusesResponse {
  const map<string, vector<string>> &buses_to_stops;
};

ostream &operator<<(ostream &os, const AllBusesResponse &r) {
  if (r.buses_to_stops.empty()) {
    os << "No buses";
  } else {
    bool firstSting = true;
    for (const auto &[bus, stops] : r.buses_to_stops) {
      if (!firstSting) {
        os << endl;
      }
      firstSting = false;
      os << "Bus " << bus << ":";
      for (const auto &s : stops) {
        os << ' ' << s;
      }
    }
  }
  return os;
}

class BusManager {
public:
  void AddBus(const string &bus, const vector<string> &stops) {
    // Реализуйте этот метод
    busToStopsMap[bus] = stops;
    for (const string &stop : stops) {
      stopToBusesMap[stop].push_back(bus);
    }
  }

  [[nodiscard]] BusesForStopResponse GetBusesForStop(const string &stop) const {
    if (stopToBusesMap.count(stop) == 0) {
      return {};
    }
    return BusesForStopResponse{stop, stopToBusesMap.at(stop)};
  }

  [[nodiscard]] StopsForBusResponse GetStopsForBus(const string &bus) const {
    // Реализуйте этот метод
    if (busToStopsMap.count(bus) == 0) {
      return {};
    }
    StopsForBusResponse response{bus};
    for (const string &stop : busToStopsMap.at(bus)) {
      if (stopToBusesMap.at(stop).size() == 1) {
        auto emptyPair = make_pair(stop, vector<string>{});
        response.interchanges.push_back(emptyPair);
      } else {
        vector<string> buses;
        for (const string &other_bus : stopToBusesMap.at(stop)) {
          if (bus != other_bus) {
            buses.push_back(other_bus);
          }
        }
        response.interchanges.emplace_back(make_pair(stop, buses));
      }
    }
    return response;
  }

  [[nodiscard]] AllBusesResponse GetAllBuses() const {
    const auto &result = busToStopsMap;
    return AllBusesResponse{result};
  }

  const map<string, vector<string>> &GetBusToStopsMap() {
    return busToStopsMap;
  }

  const map<string, vector<string>> &GetStopToBusesMap() {
    return stopToBusesMap;
  }

private:
  map<string, vector<string>> busToStopsMap, stopToBusesMap;
};

// Не меняя тела функции main, реализуйте функции и классы выше
template <typename T> void PrintVector(const vector<T> &container) {
  cout << "[";
  bool first = true;
  for (const auto &i : container) {
    if (!first) {
      cout << ", ";
    }
    first = false;
    cout << i;
  }
  cout << "]";
}

void TestAddBus() {
  BusManager bm;
  Query bus32{
      QueryType::NewBus, "32", "", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}};
  Query bus32k{QueryType::NewBus,
               "32K",
               "",
               {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino",
                "Solntsevo", "Skolkovo"}};
  Query bus950{QueryType::NewBus,
               "950",
               "",
               {"Kokoshkino", "Marushkino", "Vnukovo", "Peredelkino",
                "Solntsevo", "Troparyovo"}};
  Query bus272{QueryType::NewBus,
               "272",
               "",
               {"Vnukovo", "Moskovsky", "Rumyantsevo", "Troparyovo"}};

  bm.AddBus(bus32.bus, bus32.stops);
  bm.AddBus(bus32k.bus, bus32k.stops);
  auto busToStopsMap = bm.GetBusToStopsMap();
  auto stopToBusesMap = bm.GetStopToBusesMap();
  vector<string> expectedBuses = {"32", "32K"};
  vector<string> expectedBus = {"32K"};

  vector<string> expectedBusesTroparevo = {"950", "272"};

  assert(busToStopsMap[bus32.bus] == bus32.stops);
  assert(busToStopsMap[bus32k.bus] == bus32k.stops);
  assert(equal(stopToBusesMap["Tolstopaltsevo"].begin(),
               stopToBusesMap["Tolstopaltsevo"].end(), expectedBuses.begin()));
  assert(equal(stopToBusesMap["Marushkino"].begin(),
               stopToBusesMap["Marushkino"].end(), expectedBuses.begin()));
  assert(equal(stopToBusesMap["Skolkovo"].begin(),
               stopToBusesMap["Skolkovo"].end(), expectedBus.begin()));
  bm.AddBus(bus950.bus, bus950.stops);
  bm.AddBus(bus272.bus, bus272.stops);
  assert(equal(stopToBusesMap["Troparyovo"].begin(),
               stopToBusesMap["Troparyovo"].end(),
               expectedBusesTroparevo.begin()));
  cout << "[PASS] TestAddBuss" << endl;
}

void TestBusesForStopResponse() {
  BusManager bm;
  Query bus32{
      QueryType::NewBus, "32", "", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}};
  Query bus32k{QueryType::NewBus,
               "32K",
               "",
               {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino",
                "Solntsevo", "Skolkovo"}};
  // Test for empty bm
  auto testResponse = bm.GetBusesForStop(bus32.stop);
  assert(testResponse.stop.empty() && testResponse.buses.empty());

  bm.AddBus(bus32.bus, bus32.stops);
  bm.AddBus(bus32k.bus, bus32k.stops);
  vector<string> expectedBuses = {"32", "32K"};
  vector<string> expectedBus = {"32K"};

  auto marushkino = bm.GetBusesForStop("Marushkino");
  assert(marushkino.stop == "Marushkino");
  assert(equal(marushkino.buses.begin(), marushkino.buses.end(),
               expectedBuses.begin()));

  auto skolkovo = bm.GetBusesForStop("Skolkovo");
  assert(skolkovo.stop == "Skolkovo");
  assert(
      equal(skolkovo.buses.begin(), skolkovo.buses.end(), expectedBus.begin()));

  cout << "[PASS] TestBusesForStopResponse" << endl;
}

void TestStopsForBusResponse() {
  BusManager bm;
  Query bus32{
      QueryType::NewBus, "32", "", {"Tolstopaltsevo", "Marushkino", "Vnukovo"}};
  Query bus32k{QueryType::NewBus,
               "32K",
               "",
               {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino",
                "Solntsevo", "Skolkovo"}};
  Query bus950{QueryType::NewBus,
               "950",
               "",
               {"Kokoshkino", "Marushkino", "Vnukovo", "Peredelkino",
                "Solntsevo", "Troparyovo"}};
  Query bus272{QueryType::NewBus,
               "272",
               "",
               {"Vnukovo", "Moskovsky", "Rumyantsevo", "Troparyovo"}};
  auto emptyResponce = bm.GetStopsForBus("Solntsevo");
  assert(emptyResponce.bus.empty() && emptyResponce.interchanges.empty());

  bm.AddBus(bus32.bus, bus32.stops);
  bm.AddBus(bus32k.bus, bus32k.stops);
  bm.AddBus(bus950.bus, bus950.stops);
  bm.AddBus(bus272.bus, bus272.stops);

  vector<pair<string, vector<string>>> expected = {
      {"Vnukovo", {"32", "32K", "950"}},
      {"Moskovsky", {}},
      {"Rumyantsevo", {}},
      {"Troparyovo", {"950"}}};
  auto response = bm.GetStopsForBus("272");
  assert(response.bus == "272");
  assert(equal(response.interchanges.begin(), response.interchanges.end(),
               expected.begin()));
  cout << "[PASS] TestStopsForBusResponse" << endl;
}

int main() {
  // Test section
  TestAddBus();
  TestBusesForStopResponse();
  TestStopsForBusResponse();

  // main code
  int query_count;
  Query q;
  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
}
// No buses
// No stop
// No bus
// 32 32K
// Stop Vnukovo: 32 32K 950
// Stop Moskovsky: no interchange
// Stop Rumyantsevo: no interchange
// Stop Troparyovo: 950
// Bus 272: Vnukovo Moskovsky Rumyantsevo Troparyovo
// Bus 32: Tolstopaltsevo Marushkino Vnukovo
// Bus 32K: Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
// Bus 950: Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo