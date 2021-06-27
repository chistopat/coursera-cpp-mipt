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
    return std::fabs(left - right) < 1e-7;
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
        result.push_back(s.substr(0, pos));
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

public:
    string name;
    Point point;
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
         auto first = route.begin();
         for (auto it = route.begin() + 1; it != route.end(); ++it) {
             length += LengthBetweenPoints(*first, *it);
             first = it;
         }
         if (!is_roundtrip) {
             length *= 2;
         }
    }
public:
    static double LengthBetweenPoints(shared_ptr<Stop> left, shared_ptr<Stop> right) {
        auto result = acos((sin(left->point.latitude) * sin(right->point.latitude)) +
                        (cos(left->point.latitude) * cos(right->point.latitude) *
             cos(fabs(left->point.longitude - right->point.longitude)))
        ) * 6371000;
        return result;
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
    string object_name;
    string message;
};
using ResponseHolder = unique_ptr<Response>;

ResponseHolder MakeResponse(const string& name, string&& message) {
    Response response{name, move(message)};
    return make_unique<Response>(move(response));
}

struct AddStopRequest : public ModifyRequest {
    AddStopRequest() : ModifyRequest(Type::ADD_STOP) {}
    void ParseFrom(string_view input) override {
        name = ReadToken(input, ":");
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
        string route_delimiter = " - ";
        if (auto it = find(input.begin(), input.end(), '-');
            it == input.end()) {
            route_delimiter = " > ";
            is_roundtrip = true;
        }
        name = ReadToken(input, ": ");
        const auto stop_view = SplitBy(input, route_delimiter);
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
    GetBusRequest() : ReadRequest<ResponseHolder>(Type::ADD_BUS) {}
    void ParseFrom(string_view input) override {
        name = input;
    }

    ResponseHolder Process(const TransportManager& manager) const override {
        const auto bus_route = manager.GetBus(name);
        if (bus_route != nullptr) {
            return MakeResponse(name, bus_route->ToString());
        } else {
            return MakeResponse(name, "not found");
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
    os << "Bus " << response->object_name << ": " << response->message;
    return os;
}

void PrintResponses(const vector<ResponseHolder>& responses, ostream& stream = cout) {
    for (const auto& response : responses) {
        stream << response << endl;
    }
}

namespace Tests {
    void AddStop() {
        TransportManager transport_manager;
        transport_manager.AddStop("a", 1.0, 0.0);
        const auto a = transport_manager.GetStop("a");

        ASSERT_EQUAL(a->name, "a")
        ASSERT(AreDoubleSame(a->point.latitude, 0.0174533))
        ASSERT(AreDoubleSame(a->point.longitude, 0.0))
    }

    void AddBus() {
        TransportManager transport_manager;
        transport_manager.AddStop("a", 55.611087, 37.20829);
        transport_manager.AddStop("b", 55.595884, 37.209755);
        transport_manager.AddStop("c", 55.632761, 37.333324);

        transport_manager.AddBus("759", true, {"a", "b", "c", "a"});
        transport_manager.AddBus("000", false, {"a", "b", "c"});

        const auto bus = transport_manager.GetBus("759");
        const auto bus2 = transport_manager.GetBus("000");

        ASSERT_EQUAL(bus->ToString(), "4 stops on route, 3 unique stops, 18681.8 route length")
        ASSERT_EQUAL(bus2->ToString(), "5 stops on route, 3 unique stops, 20939.5 route length")
    }

    void Smoke() {
        std::istringstream is;
        const std::string input = "10\n"
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
                                  "3\n"
                                  "Bus 256\n"
                                  "Bus 750\n"
                                  "Bus 751\n";
        is.str(input);

        const string output = "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
                              "Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
                              "Bus 751: not found\n";

        ostringstream os;

        TransportManager manager;
        const auto requests_to_write = ReadRequests(DataBaseMode::WRITE, is);
        const auto requests_to_read = ReadRequests(DataBaseMode::READ, is);
        ProcessWriteRequests(requests_to_write, manager);
        const auto responses = ProcessReadRequests(requests_to_read, manager);
        PrintResponses(responses, os);
        auto result = os.str();
        ASSERT_EQUAL(result, output)
    }
}
void TestAll() {
    TestRunner test_runner;
    RUN_TEST(test_runner, Tests::AddBus);
    RUN_TEST(test_runner, Tests::AddStop);
    RUN_TEST(test_runner, Tests::Smoke);
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