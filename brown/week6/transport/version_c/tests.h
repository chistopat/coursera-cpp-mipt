#pragma once


namespace Tests {
    void SetUp(istream& in, ostream& out) {
        ostringstream log;
        TransportManager manager;
        const auto requests_to_write = ReadRequests(DataBaseMode::WRITE, log, in);
        const auto requests_to_read = ReadRequests(DataBaseMode::READ, log, in);
        ProcessWriteRequests(requests_to_write, manager);
        const auto responses = ProcessReadRequests(requests_to_read, manager);
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

}// Tests NS