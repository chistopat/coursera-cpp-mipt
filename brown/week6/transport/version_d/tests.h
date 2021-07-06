#pragma once

#include "debug.h"

namespace Tests {
    void SetUp(istream& in, ostream& out) {
        ostringstream log;
        TransportManager manager;
        const auto requests = ReadRequests(in, DEBUG, TOKEN);
        const auto responses = ProcessRequests(requests, manager);
        PrintResponses(responses, out);
    }

    void CheckStrip() {
        ASSERT_EQUAL("as df", Strip({"   as df   "}));
    }
    void AddStop() {
        TransportManager transport_manager;
        transport_manager.AddStop("a", 1.0, 0.0);
        const auto a = transport_manager.GetStop("a");

        ASSERT_EQUAL(a->name, "a")
        ASSERT(AreDoubleSame(a->point.latitude, 0.0174533))
        ASSERT(AreDoubleSame(a->point.longitude, 0.0))
    }

    void AddStopSpaces() {
        const double x = 55.611087;
        const double y = 37.20829;
        TransportManager transport_manager;
        transport_manager.AddStop("a", 1, 1);
        transport_manager.AddStop(" a", 2, 2);
        transport_manager.AddStop("a ", 3, 3);
        transport_manager.AddStop(" a ", 4, 4);
        transport_manager.AddStop("a a", 5, 5);
        transport_manager.AddStop("a   a", 6, 6);

        ASSERT_EQUAL(to_string(transport_manager.GetStop("a")->point.latitude), "0.017453");
        ASSERT_EQUAL(to_string(transport_manager.GetStop(" a")->point.latitude), "0.017453");
        ASSERT_EQUAL(to_string(transport_manager.GetStop("a ")->point.latitude), "0.017453");
        ASSERT_EQUAL(to_string(transport_manager.GetStop(" a ")->point.latitude), "0.017453");
        ASSERT_EQUAL(to_string(transport_manager.GetStop("a a")->point.latitude), "0.017453");
        ASSERT_EQUAL(to_string(transport_manager.GetStop("a   a")->point.latitude), "0.017453");
    }

    void AddBus() {
        TransportManager transport_manager;
        transport_manager.AddStop("a a", 55.611087, 37.20829);
        transport_manager.AddStop("b b", 55.595884, 37.209755);
        transport_manager.AddStop("c c", 55.632761, 37.333324);
        transport_manager.AddStop("d d", 55.632761, 37.333324);

        transport_manager.AddBus("759", true, {"a a", "b b", "c c", "a a"});
        transport_manager.AddBus("000", false, {"a a", "b b", "c c"});

        const auto bus = transport_manager.GetBus("759");
        const auto bus2 = transport_manager.GetBus("000");

        ASSERT_EQUAL(bus->ToString(), "4 stops on route, 3 unique stops, 0 route length, 0 courvature")
        ASSERT_EQUAL(bus2->ToString(), "5 stops on route, 3 unique stops, 0 route length, 0 courvature")

        const auto stop_a = transport_manager.GetStop("a a");
        const auto stop_d = transport_manager.GetStop("d d");
        const auto stop_f = transport_manager.GetStop("f");
        const set<string> expected_buses = {"000", "759"};
        ASSERT_EQUAL(stop_a->buses, expected_buses);
        ASSERT_EQUAL(stop_d->buses, set<string>{});
        ASSERT(!stop_f.has_value());
    }

    void LengthBetweenSamePoints() {
        const double x = 55.611087;
        const double y = 37.20829;

        auto len = BusRoute::LengthBetweenPoints(make_shared<Stop>("a", x, y), make_shared<Stop>("b", x, y));

        ASSERT_EQUAL(len, 0);
    }

    void AddBusSameStops() {
        TransportManager transport_manager;
        const double x = 55.611087;
        const double y = 37.20829;
        const string name = "a";
        transport_manager.AddStop(name, x, y);
        transport_manager.AddStop(name, x, y);
        transport_manager.AddStop(name, x, y);

        transport_manager.AddBus("111", true, {"a", "a", "a"});
        transport_manager.AddBus("222", false, {"a", "a", "a"});

        const auto bus = transport_manager.GetBus("111");
        const auto bus2 = transport_manager.GetBus("222");

        ASSERT_EQUAL(bus->ToString(), "3 stops on route, 1 unique stops, 0 route length, nan courvature")
        ASSERT_EQUAL(bus2->ToString(), "5 stops on route, 1 unique stops, 0 route length, nan courvature")
        //
        //
        //        const auto stop_a = transport_manager.GetStop("a a");
        //        const auto stop_d = transport_manager.GetStop("d d");
        //        const auto stop_f = transport_manager.GetStop("f");
        //        const set<string> expected_buses = {"000", "759"};
        //        ASSERT_EQUAL(stop_a->buses, expected_buses);
        //        ASSERT_EQUAL(stop_d->buses, set<string>{});
        //        ASSERT(!stop_f.has_value());
    }

    void Smoke() {
        std::istringstream is;
        const std::string input =
            "13\n"
            "Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino\n"
            "Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka\n"
            "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
            "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
            "Stop Rasskazovka: 55.632761, 37.333324\n"
            "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam\n"
            "Stop Biryusinka: 55.581065, 37.64839, 750m to Universam\n"
            "Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya\n"
            "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya\n"
            "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye\n"
            "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
            "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
            "Stop Prazhskaya: 55.611678, 37.603831\n"
            "6\n"
            "Bus 256\n"
            "Bus 750\n"
            "Bus 751\n"
            "Stop Samara\n"
            "Stop Prazhskaya\n"
            "Stop Biryulyovo Zapadnoye\n";

        is.str(input);

        const string output = "Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.361239 curvature\n"
                              "Bus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.318084 curvature\n"
                              "Bus 751: not found\n"
                              "Stop Samara: not found\n"
                              "Stop Prazhskaya: no buses\n"
                              "Stop Biryulyovo Zapadnoye: buses 256 828\n";

        ostringstream out;
        out.precision(6);
        SetUp(is, out);
        auto result = out.str();
        ASSERT_EQUAL(result, output)
    }

    void FuzzNames() {
        std::istringstream is;
        const std::string input = "4\n"
                                  "Bus №666-кольцевой автобус: пердово > новые васюки > пердово\n"
                                  "Bus №000>экспресс: пердово - новые васюки\n"
                                  "Stop пердово: 55.611087, 37.20829\n"
                                  "Stop новые васюки: 55.595884, 37.209755\n"
                                  "4\n"
                                  "Stop пердово\n"
                                  "Bus №666-кольцевой автобус\n"
                                  "Stop новые васюки\n"
                                  "Bus №000>экспресс\n";

        const string output = "Stop пердово: buses №000>экспресс №666-кольцевой автобус\n"
                              "Bus №666-кольцевой автобус: 3 stops on route, 2 unique stops, 0 route length, 0 courvature\n"
                              "Stop новые васюки: buses №000>экспресс №666-кольцевой автобус\n"
                              "Bus №000>экспресс: 3 stops on route, 2 unique stops, 0 route length, 0 courvature\n";
        is.str(input);
        ostringstream out;

        SetUp(is, out);
        auto result = out.str();
        ASSERT_EQUAL(result, output)
    }

    void TestEmptyBase() {
        const std::string input = "0\n"
                                  "2\n"
                                  "Stop пердово\n"
                                  "Stop новые васюки\n";

        const string output = "Stop пердово: not found\n"
                              "Stop новые васюки: not found\n";
        std::istringstream is;
        std::ostringstream os;
        is.str(input);

        SetUp(is, os);
        auto result = os.str();
        ASSERT_EQUAL(result, output)
    }

    void ParseStopRequest() {
        AddStopRequest stopRequest;
        stopRequest.ParseFrom("Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam");
        ASSERT_EQUAL(stopRequest.name, "Biryulyovo Zapadnoye");
        ASSERT_EQUAL(stopRequest.distance_map.values["Rossoshanskaya ulitsa"], 7500);
        ASSERT_EQUAL(stopRequest.distance_map.values["Biryusinka"], 1800);
        ASSERT_EQUAL(stopRequest.distance_map.values["Universam"], 2400);
    }

    void JsonSmoke() {
        const string json_input = R"({
          "base_requests": [
            {
              "type": "Stop",
              "road_distances": {
                "Marushkino": 3900
              },
              "longitude": 37.20829,
              "name": "Tolstopaltsevo",
              "latitude": 55.611087
            },
            {
              "type": "Stop",
              "road_distances": {
                "Rasskazovka": 9900
              },
              "longitude": 37.209755,
              "name": "Marushkino",
              "latitude": 55.595884
            },
            {
              "type": "Bus",
              "name": "256",
              "stops": [
                "Biryulyovo Zapadnoye",
                "Biryusinka",
                "Universam",
                "Biryulyovo Tovarnaya",
                "Biryulyovo Passazhirskaya",
                "Biryulyovo Zapadnoye"
              ],
              "is_roundtrip": true
            },
            {
              "type": "Bus",
              "name": "750",
              "stops": [
                "Tolstopaltsevo",
                "Marushkino",
                "Rasskazovka"
              ],
              "is_roundtrip": false
            },
            {
              "type": "Stop",
              "road_distances": {},
              "longitude": 37.333324,
              "name": "Rasskazovka",
              "latitude": 55.632761
            },
            {
              "type": "Stop",
              "road_distances": {
                "Rossoshanskaya ulitsa": 7500,
                "Biryusinka": 1800,
                "Universam": 2400
              },
              "longitude": 37.6517,
              "name": "Biryulyovo Zapadnoye",
              "latitude": 55.574371
            },
            {
              "type": "Stop",
              "road_distances": {
                "Universam": 750
              },
              "longitude": 37.64839,
              "name": "Biryusinka",
              "latitude": 55.581065
            },
            {
              "type": "Stop",
              "road_distances": {
                "Rossoshanskaya ulitsa": 5600,
                "Biryulyovo Tovarnaya": 900
              },
              "longitude": 37.645687,
              "name": "Universam",
              "latitude": 55.587655
            },
            {
              "type": "Stop",
              "road_distances": {
                "Biryulyovo Passazhirskaya": 1300
              },
              "longitude": 37.653656,
              "name": "Biryulyovo Tovarnaya",
              "latitude": 55.592028
            },
            {
              "type": "Stop",
              "road_distances": {
                "Biryulyovo Zapadnoye": 1200
              },
              "longitude": 37.659164,
              "name": "Biryulyovo Passazhirskaya",
              "latitude": 55.580999
            },
            {
              "type": "Bus",
              "name": "828",
              "stops": [
                "Biryulyovo Zapadnoye",
                "Universam",
                "Rossoshanskaya ulitsa",
                "Biryulyovo Zapadnoye"
              ],
              "is_roundtrip": true
            },
            {
              "type": "Stop",
              "road_distances": {},
              "longitude": 37.605757,
              "name": "Rossoshanskaya ulitsa",
              "latitude": 55.595579
            },
            {
              "type": "Stop",
              "road_distances": {},
              "longitude": 37.603831,
              "name": "Prazhskaya",
              "latitude": 55.611678
            }
          ],
          "stat_requests": [
            {
              "type": "Bus",
              "name": "256",
              "id": 1965312327
            },
            {
              "type": "Bus",
              "name": "750",
              "id": 519139350
            },
            {
              "type": "Bus",
              "name": "751",
              "id": 194217464
            },
            {
              "type": "Stop",
              "name": "Samara",
              "id": 746888088
            },
            {
              "type": "Stop",
              "name": "Prazhskaya",
              "id": 65100610
            },
            {
              "type": "Stop",
              "name": "Biryulyovo Zapadnoye",
              "id": 1042838872
            }
          ]
        }
    )";
        istringstream is;
        is.str(json_input);
        ostringstream out;
        out.precision(6);
        SetUp(is, cout);
//        auto result = out.str();
//        ASSERT_EQUAL(result, output)
    }


    void Test3() {

        const string json_input = R"(
          {
  	"base_requests": [{
  		"type": "Bus",
  		"name": "vW1R0hGBlnaSNo6M58N3",
  		"stops": ["FX06Xkvth9wCRvAUZ", "0f3", "FX06Xkvth9wCRvAUZ"],
  		"is_roundtrip": true
  	}, {
  		"type": "Stop",
  		"name": "0f3",
  		"latitude": 38.42309720084585,
  		"longitude": 34.73119278667983,
  		"road_distances": {
  			"FX06Xkvth9wCRvAUZ": 914534
  		}
  	}, {
  		"type": "Bus",
  		"name": "kXbq1NGXQFmAfiYYL",
  		"stops": ["0f3", "FX06Xkvth9wCRvAUZ", "0f3"],
  		"is_roundtrip": true
  	}, {
  		"type": "Stop",
  		"name": "FX06Xkvth9wCRvAUZ",
  		"latitude": 38.413522679752795,
  		"longitude": 34.77839058726191,
  		"road_distances": {
  			"0f3": 674452
  		}
  	}, {
  		"type": "Bus",
  		"name": "8Fzx3Q",
  		"stops": ["0f3", "FX06Xkvth9wCRvAUZ", "0f3"],
  		"is_roundtrip": true
  	}],
  	"stat_requests": [{
  		"id": 1529842013,
  		"type": "Stop",
  		"name": "G3CI8GVWJuzWKu"
  	}, {
  		"id": 992995459,
  		"type": "Stop",
  		"name": "0f3"
  	}, {
  		"id": 1993746106,
  		"type": "Bus",
  		"name": "vW1R0hGBlnaSNo6M58N3"
  	}, {
  		"id": 1993746106,
  		"type": "Bus",
  		"name": "vW1R0hGBlnaSNo6M58N3"
  	}, {
  		"id": 1777317630,
  		"type": "Stop",
  		"name": "FX06Xkvth9wCRvAUZ"
  	}, {
  		"id": 992995459,
  		"type": "Stop",
  		"name": "0f3"
  	}, {
  		"id": 553304384,
  		"type": "Bus",
  		"name": "D12sVwF2FOANXi"
  	}, {
  		"id": 1993746106,
  		"type": "Bus",
  		"name": "vW1R0hGBlnaSNo6M58N3"
  	}, {
  		"id": 550798604,
  		"type": "Bus",
  		"name": "RDBiZwD"
  	}, {
  		"id": 2002102572,
  		"type": "Bus",
  		"name": "8Fzx3Q"
  	}]
  }
        )";

        istringstream is;
        is.str(json_input);
        ostringstream out;
        out.precision(6);
        SetUp(is, cout);
    }

}// Tests NS