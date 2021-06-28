#include "test_runner.h"

#include <cmath>
#include <array>
#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

bool AreDoubleSame(double left, double right)
{
    return std::fabs(left - right) < 1e-5;
}


string_view Strip(string_view s) {
    while (!s.empty() && isspace(s.front())) {
        s.remove_prefix(1);
    }
    while (!s.empty() && isspace(s.back())) {
        s.remove_suffix(1);
    }
    return s;
}

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ") {
    const size_t pos = s.find(delimiter);
    if (pos == string::npos) {
        return {s, nullopt};
    } else {
        return {s.substr(0, pos), s.substr(pos + delimiter.length())};
    }
}

pair<string_view, string_view> SplitTwo(string_view s, string_view delimiter = " ") {
    const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
    return {lhs, rhs_opt.value_or("")};
}

string_view ReadToken(string_view& s, string_view delimiter = " ") {
    const auto [lhs, rhs] = SplitTwo(s, delimiter);
    s = rhs;
    return lhs;
}

template <typename Number>
Number ReadNumberOnLine(istream& stream) {
    Number number;
    stream >> number;
    string dummy;
    getline(stream, dummy);
    return number;
}

vector<string_view> SplitBy(string_view s, string_view delimiter) {
    vector<string_view> result;
    while (!s.empty()) {
        size_t pos = s.find(delimiter);
        result.push_back(Strip(s.substr(0, pos)));
        s.remove_prefix(pos != string::npos ? pos + delimiter.length() : s.size());
    }
    return result;
}

class Point {

public:
    static constexpr double halfC = 3.14159265358979323846 / 180;

public:
    explicit Point(double lat,  double lon)
        : latitude(lat * halfC)
        , longitude(lon * halfC) {
    }

    double latitude;
    double longitude;

public:

};

bool operator==(const Point& left, const Point& right) {
    return std::tie(left.latitude, left.longitude) == std::tie(right.latitude, right.longitude);
}

ostream& operator<< (ostream& os, const Point& point) {
    os << point.latitude << " " << point.longitude;
    return os;
}

class Stop {
public:
    static unique_ptr<Stop> Create(const string& name, double latitude, double longitude) {
        Stop stop(name, latitude, longitude);
        return make_unique<Stop>(move(stop));
    }

public:
    Stop(const string& name, double lat, double lon)
        : name(name)
        , point(lat, lon) {

    }
    string ToString() const {
        ostringstream os;
        if (buses.empty()) {
            return "no buses";
        }
        os << "buses ";
        bool first = true;
        for (const auto& bus : buses) {
            if (!first) {
                os << " ";
            }
            os << bus;
            first = false;
        }
        return os.str();
    }

public:
    string name;
    Point point;
    set<string> buses;
};

ostream& operator<< (ostream& os, const Stop& stop) {
    os << stop.name << ": " << stop.point.latitude << " " << stop.point.longitude;
    return os;
}

class BusRoute {
public:
    static unique_ptr<BusRoute> MakeBusRoute(const string& name,  bool is_roundtrip,
                                             const vector<shared_ptr<Stop>>& stop_list) {
        BusRoute bus_route(name, is_roundtrip, stop_list);
        return make_unique<BusRoute>(move(bus_route));
    }

    BusRoute(const string& name, bool is_roundtrip, const vector<shared_ptr<Stop>>& stop_list)
        : is_roundtrip(is_roundtrip)
        , name(name)
        , length(0)
        , total_stops_count(0)
        , unique_stops_count(0)
        , route(stop_list)
    {
        ComputeRouteLength();
        SetTotalStopsCount();
        SetUniqueStopsCount();
    }

    string ToString() const {
        ostringstream os;
        os << total_stops_count << " stops on route, "
           << unique_stops_count << " unique stops, "
           << length <<  " route length";
        return os.str();
    }

    bool is_roundtrip;
    string name;
    double length;
    size_t total_stops_count;
    size_t unique_stops_count;
    vector<shared_ptr<Stop>> route;

private:
    void SetTotalStopsCount() {
        if (is_roundtrip) {
            total_stops_count = route.size();
        } else {
            total_stops_count = (route.size() * 2) - 1;
        }
    }

    void SetUniqueStopsCount() {
        set<string> uniq_names;
        for (const auto& stop : route) {
            uniq_names.insert(stop->name);
        }
        unique_stops_count = uniq_names.size();
    }

    void ComputeRouteLength() {
        length = ComputeOneLineLength(route.begin(), route.end());
         if (!is_roundtrip) {
             length += ComputeOneLineLength(route.rbegin(), route.rend());;
         }
    }
public:
    static double LengthBetweenPoints(shared_ptr<Stop> left, shared_ptr<Stop> right) {
//        if (left->point == right->point) {
//            return 0;
//        }
        auto result = acos((sin(left->point.latitude) * sin(right->point.latitude)) +
                        (cos(left->point.latitude) * cos(right->point.latitude) *
             cos(fabs(left->point.longitude - right->point.longitude)))
        ) * 6371000;
        return result;
    }

    template <typename It>
    static double ComputeOneLineLength(It begin, It end) {
        double length = 0;
        auto first = begin;
        for (auto it = begin + 1; it != end; ++it) {
            length += LengthBetweenPoints(*first, *it);
            first = it;
        }
        return length;
    }
};

class TransportManager {
public:
    TransportManager() = default;

    void AddStop(const string& name, double latitude, double longitude) {
        if (!stops_dictonary_.count(name)) {
            stops_dictonary_.insert({name, Stop::Create(name, latitude, longitude)});
        }
    }

    void AddBus(const string& name, bool is_roundtrip, const vector<string>& stop_list) {
        if (!bus_routes_dictonary_.count(name)) {
            bus_routes_dictonary_.insert({name, BusRoute::MakeBusRoute(name, is_roundtrip, GetStopList(stop_list))});
            for (auto& stop : bus_routes_dictonary_[name]->route) {
                stop->buses.insert(name);
            }
        }
    }

    const optional<Stop> GetStop(const string& name) const {
        if (stops_dictonary_.count(name)) {
            return *stops_dictonary_.at(name);
        }
        return nullopt;
    }

    shared_ptr<BusRoute> GetBus(const string& name) const {
        if (bus_routes_dictonary_.count(name)) {
            return bus_routes_dictonary_.at(name);
        }
        return nullptr;
    }

    vector<shared_ptr<Stop>> GetStopList(const vector<string>& stop_list) const {
        vector<shared_ptr<Stop>> result;
        result.reserve(stop_list.size());
        for (const auto& stop : stop_list) {
            result.push_back(stops_dictonary_.at(stop));
        }
        return result;
    }

private:
    unordered_map<string, shared_ptr<Stop>> stops_dictonary_;
    unordered_map<string, shared_ptr<BusRoute>> bus_routes_dictonary_;
};


struct Request;
using RequestHolder = unique_ptr<Request>;

struct Request {

    enum class Type {
        ADD_STOP,
        ADD_BUS,
        GET_BUS,
        GET_STOP,
        LAST_,
    };

    Request(Type type) : type(type) {}
    static RequestHolder Create(Type type);
    virtual void ParseFrom(string_view input) = 0;
    virtual ~Request() = default;

    const Type type;
};

static constexpr auto REQUESTS_TYPE_COUNT = static_cast<size_t>(Request::Type::LAST_);
using RequestMap = const unordered_map<string_view, Request::Type>;

RequestMap STR_TO_MODIFY_REQUEST_TYPE = {
    {"Bus", Request::Type::ADD_BUS},
    {"Stop", Request::Type::ADD_STOP},
};


RequestMap STR_TO_READ_REQUEST_TYPE = {
    {"Bus", Request::Type::GET_BUS},
    {"Stop", Request::Type::GET_STOP},
};

enum class DataBaseMode {
    READ,
    WRITE
};


template <typename ResultType>
struct ReadRequest : Request {

    using Request::Request;
    virtual ResultType Process(const TransportManager& manager) const = 0;
};

struct ModifyRequest : Request {

    using Request::Request;
    virtual void Process(TransportManager& manager) const = 0;
};

struct Response {
    string object_type;
    string object_name;
    string message;
};
using ResponseHolder = unique_ptr<Response>;

ResponseHolder MakeResponse(const string& type, const string& name, string&& message) {
    Response response{type, name, move(message)};
    return make_unique<Response>(move(response));
}


struct AddStopRequest : public ModifyRequest {
    AddStopRequest() : ModifyRequest(Type::ADD_STOP) {}
    void ParseFrom(string_view input) override {
        name = Strip(ReadToken(input, ":"));
        latitude = stod(string(ReadToken(input, ",")));
        longitude = stod(string(input));
    }

    void Process(TransportManager& manager) const override {
        return manager.AddStop(name, latitude, longitude);
    }

    string name;
    double latitude;
    double longitude;
};

struct AddBusRequest : public ModifyRequest {
    AddBusRequest()
        : ModifyRequest(Type::ADD_BUS)
        , is_roundtrip(false) {}
    void ParseFrom(string_view input) override {
        name = Strip(ReadToken(input, ": "));
        string route_delimiter = " - ";
        if (auto it = find(input.begin(), input.end(), '-');
            it == input.end()) {
            route_delimiter = " > ";
            is_roundtrip = true;
        }
        const auto stop_view = SplitBy(Strip(input), route_delimiter);
        stop_list.insert(stop_list.end(), stop_view.begin(), stop_view.end());
    }

    void Process(TransportManager& manager) const override {
        return manager.AddBus(name, is_roundtrip, stop_list);
    }

    string name;
    bool is_roundtrip;
    vector<string> stop_list;
};

struct GetBusRequest : public ReadRequest<ResponseHolder> {
    GetBusRequest() : ReadRequest<ResponseHolder>(Type::GET_BUS) {}
    void ParseFrom(string_view input) override {
        name = Strip(input);
    }

    ResponseHolder Process(const TransportManager& manager) const override {
        const auto bus_route = manager.GetBus(name);
        if (bus_route != nullptr) {
            return MakeResponse("Bus", name, bus_route->ToString());
        } else {
            return MakeResponse("Bus", name, "not found");
        }
    }
    string name;
};

struct GetStopRequest : public ReadRequest<ResponseHolder> {
    GetStopRequest() : ReadRequest<ResponseHolder>(Type::GET_STOP) {}
    void ParseFrom(string_view input) override {
        name = Strip(input);
    }

    ResponseHolder Process(const TransportManager& manager) const override {
        const auto stop = manager.GetStop(name);
        if (stop.has_value()) {
            return MakeResponse("Stop", name, stop->ToString());
        } else {
            return MakeResponse("Stop", name, "not found");
        }
    }

    string name;
};

RequestHolder Request::Create(Request::Type type) {
    switch (type) {
        case Request::Type::ADD_STOP:
            return make_unique<AddStopRequest>();
        case Request::Type::ADD_BUS:
            return make_unique<AddBusRequest>();
        case Request::Type::GET_BUS:
            return make_unique<GetBusRequest>();
        case Request::Type::GET_STOP:
            return make_unique<GetStopRequest>();
        default:
            return nullptr;
    }
}

optional<Request::Type> ConvertRequestTypeFromString(string_view type_str, DataBaseMode mode) {
    if (mode == DataBaseMode::READ) {
        if (const auto it = STR_TO_READ_REQUEST_TYPE.find(type_str);
            it != STR_TO_READ_REQUEST_TYPE.end()) {
            return it->second;
        }
        return nullopt;
    } else if (mode == DataBaseMode::WRITE) {
        if (const auto it = STR_TO_MODIFY_REQUEST_TYPE.find(type_str);
            it != STR_TO_MODIFY_REQUEST_TYPE.end()) {
            return it->second;
        }
        return nullopt;
    } else {
        return nullopt;
    }
}

RequestHolder ParseRequest(DataBaseMode mode, string_view request_str) {
    const auto request_type = ConvertRequestTypeFromString(ReadToken(request_str), mode);
    if (!request_type) {
        return nullptr;
    }

    RequestHolder request = Request::Create(*request_type);
    if (request != nullptr) {
        request->ParseFrom(request_str);
    }

    return request;
}
array<vector<RequestHolder>, REQUESTS_TYPE_COUNT> ReadRequests(DataBaseMode mode, istream& in_stream = cin) {
    const auto request_count = ReadNumberOnLine<size_t>(in_stream);

    array<vector<RequestHolder>, REQUESTS_TYPE_COUNT> requests;
    for (auto& bucket : requests) {
        bucket.reserve(request_count);
    }

    for (size_t i = 0; i < request_count; ++i) {
        string request_str;
        getline(in_stream, request_str);
        if (auto request = ParseRequest(mode, request_str)) {
            requests[static_cast<size_t>(request->type)].push_back(move(request));
        }
    }
    return requests;
}

vector<ResponseHolder> ProcessReadRequests(const array<vector<RequestHolder>, REQUESTS_TYPE_COUNT>& requests,
                                       TransportManager& manager) {
    vector<ResponseHolder> responses;
        for (const auto& bucket : requests) {
            for (const auto& request_holder : bucket) {
                const auto& request = dynamic_cast<const ReadRequest<ResponseHolder>&>(*request_holder);
                responses.push_back(request.Process(manager));
            }
        }
    return responses;
}

void ProcessWriteRequests(const array<vector<RequestHolder>, REQUESTS_TYPE_COUNT>& requests,
                                           TransportManager& manager) {
    for (const auto& bucket : requests) {
        for (const auto& request_holder : bucket) {
            const auto& request = dynamic_cast<const ModifyRequest&>(*request_holder);
            request.Process(manager);
        }
    }
}

ostream& operator<< (ostream& os, const ResponseHolder& response) {
    os << response->object_type << " " << response->object_name << ": " << response->message;
    return os;
}

void PrintResponses(const vector<ResponseHolder>& responses, ostream& stream = cout) {
    for (const auto& response : responses) {
        stream << response << endl;
    }
}

namespace Tests {
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

        ASSERT_EQUAL(bus->ToString(), "4 stops on route, 3 unique stops, 18681.8 route length")
        ASSERT_EQUAL(bus2->ToString(), "5 stops on route, 3 unique stops, 20939.5 route length")

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

        auto len = BusRoute::LengthBetweenPoints(make_shared<Stop>("a", x, y),
            make_shared<Stop>("b", x, y));

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

        ASSERT_EQUAL(bus->ToString(), "3 stops on route, 1 unique stops, 0 route length")
        ASSERT_EQUAL(bus2->ToString(), "5 stops on route, 1 unique stops, 0 route length")
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
        const std::string input = "13\n"
            "Stop Tolstopaltsevo: 55.611087, 37.20829\n"
            "Stop Marushkino: 55.595884, 37.209755\n"
            "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
            "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
            "Stop Rasskazovka: 55.632761, 37.333324\n"
            "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n"
            "Stop Biryusinka: 55.581065, 37.64839\n"
            "Stop Universam: 55.587655, 37.645687\n"
            "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
            "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
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

        const string output = "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
                              "Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
                              "Bus 751: not found\n"
                              "Stop Samara: not found\n"
                              "Stop Prazhskaya: no buses\n"
                              "Stop Biryulyovo Zapadnoye: buses 256 828\n";

        ostringstream out;

        TransportManager manager;
        const auto requests_to_write = ReadRequests(DataBaseMode::WRITE, is);
        const auto requests_to_read = ReadRequests(DataBaseMode::READ, is);
        ProcessWriteRequests(requests_to_write, manager);
        const auto responses = ProcessReadRequests(requests_to_read, manager);
        PrintResponses(responses, out);
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
                                  "Bus №666-кольцевой автобус\n"
                                  "Bus №000>экспресс\n"
                                  "Stop пердово\n"
                                  "Stop новые васюки\n";

        const string output = "Bus №666-кольцевой автобус: 3 stops on route, 2 unique stops, 3386 route length\n"
                              "Bus №000>экспресс: 3 stops on route, 2 unique stops, 3386 route length\n"
                              "Stop пердово: buses №000>экспресс №666-кольцевой автобус\n"
                              "Stop новые васюки: buses №000>экспресс №666-кольцевой автобус\n";
        is.str(input);
        ostringstream out;

        TransportManager manager;
        const auto requests_to_write = ReadRequests(DataBaseMode::WRITE, is);
        const auto requests_to_read = ReadRequests(DataBaseMode::READ, is);
        ProcessWriteRequests(requests_to_write, manager);
        const auto responses = ProcessReadRequests(requests_to_read, manager);
        PrintResponses(responses, out);
        auto result = out.str();
        ASSERT_EQUAL(result, output)
    }


}
void TestAll() {
    TestRunner test_runner;
    RUN_TEST(test_runner, Tests::AddBus);
    RUN_TEST(test_runner, Tests::AddStop);
    RUN_TEST(test_runner, Tests::Smoke);
    RUN_TEST(test_runner, Tests::AddBusSameStops);
    RUN_TEST(test_runner, Tests::LengthBetweenSamePoints);
//    RUN_TEST(test_runner, Tests::AddStopSpaces);
    RUN_TEST(test_runner, Tests::CheckStrip);
    RUN_TEST(test_runner, Tests::FuzzNames);
}

int main() {
//    TestAll();
    cout.precision(6);
    TransportManager manager;
    const auto requests_to_write = ReadRequests(DataBaseMode::WRITE, cin);
    const auto requests_to_read = ReadRequests(DataBaseMode::READ, cin);
    ProcessWriteRequests(requests_to_write, manager);
    const auto responses = ProcessReadRequests(requests_to_read, manager);
    PrintResponses(responses, cout);

    return 0;
}