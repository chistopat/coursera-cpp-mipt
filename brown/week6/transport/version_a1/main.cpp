#include "test_runner.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <system_error>
#include <type_traits>
#include <unordered_map>
#include <vector>

using namespace std;

template<typename It>
class Range {
public:
    Range(It begin, It end) : begin_(begin), end_(end) {}
    It begin() const { return begin_; }
    It end() const { return end_; }

private:
    It begin_;
    It end_;
};

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ") {
    const size_t pos = s.find(delimiter);
    if (pos == s.npos) {
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
Number ConvertToNumber(string_view str) {
    // use std::from_chars when available to git rid of string copy
    size_t pos;
    const int result = stod(string(str), &pos);
    if (pos != str.length()) {
        std::stringstream error;
        error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
        throw invalid_argument(error.str());
    }
    return static_cast<Number>(result);
}

template <typename Number>
void ValidateBounds(Number number_to_check, Number min_value, Number max_value) {
    if (number_to_check < min_value || number_to_check > max_value) {
        std::stringstream error;
        error << number_to_check << " is out of [" << min_value << ", " << max_value << "]";
        throw out_of_range(error.str());
    }
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
        s.remove_prefix(pos != s.npos ? pos + delimiter.length() : s.size());
    }
    return result;
}

enum class BusRouteDirection {
    ROUND,
    ROTATE
};

struct Stop {
    static shared_ptr<Stop> MakeStop(const string& name, double latitude, double longitude) {
        Stop stop(name, latitude, longitude);
        return make_shared<Stop>(move(stop));
    }

    Stop(const string& name, double latitude, double longitude)
        : name(name)
        , latitude(latitude)
        , longitude(longitude) {}
    string name;
    double latitude;
    double longitude;
};


class BusRoute {
public:
    static shared_ptr<BusRoute> MakeBusRoute(const string& name, BusRouteDirection direction,
                                             const vector<shared_ptr<Stop>>& stop_list) {
        BusRoute bus_route(name, direction, stop_list);
        return make_shared<BusRoute>(move(bus_route));
    }

    BusRoute(const string& name, BusRouteDirection direction, const vector<shared_ptr<Stop>>& stop_list)
      : direction(direction)
      , name(name)
      , length(0)
      , total_stops_count(0)
      , unique_stops_count(0)
      , route(stop_list)
    {
        SetTotalStopsCount();
        SetUniqueStopsCount();
    }

    string ToString() {
        ostringstream os;
        os << total_stops_count << " stops on route, "
           << unique_stops_count << " unique stops, "
           << length <<  " route length";
        return os.str();
    }

    BusRouteDirection direction;
    string name;
    double length;
    size_t total_stops_count;
    size_t unique_stops_count;
    vector<shared_ptr<Stop>> route;

private:
    void SetTotalStopsCount() {
        switch (direction) {
            case (BusRouteDirection::ROUND):
                total_stops_count = route.size();
                break;
            case (BusRouteDirection::ROTATE):
                total_stops_count = (route.size() * 2) - 1;
                break;
        }
    }

    void SetUniqueStopsCount() {
        set<string> uniq_names;
        for (const auto& stop : route) {
            uniq_names.insert(stop->name);
        }
        unique_stops_count = uniq_names.size();
    }
};


class TransportManager {
public:
    TransportManager() = default;
    void AddStop(const string& name, double latitude, double longitude) {
        if (!stops_dictonary_.count(name)) {
            stops_dictonary_.insert({name, Stop::MakeStop(name, latitude, longitude)});
        }
    }
    void AddBus(const string& name, BusRouteDirection direction, const vector<string>& stop_list) {

        if (!bus_routes_dictonary_.count(name)) {
            bus_routes_dictonary_.insert({name, BusRoute::MakeBusRoute(name, direction, GetStopList(stop_list))});
        }
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
        latitude = ConvertToNumber<double>(ReadToken(input, ","));
        longitude = ConvertToNumber<double>(input);
    }

    void Process(TransportManager& manager) const override {
        return manager.AddStop(name, latitude, longitude);
    }

    string name;
    double latitude;
    double longitude;
};

struct AddBusRequest : public ModifyRequest {
    AddBusRequest() : ModifyRequest(Type::ADD_BUS) {}
    void ParseFrom(string_view input) override {
        string route_delimiter = " - ";
        direction = BusRouteDirection::ROTATE;
        if (auto it = find(input.begin(), input.end(), '-');
            it == input.end()) {
            route_delimiter = " > ";
            direction = BusRouteDirection::ROUND;
        }
        name = ReadToken(input, ": ");
        const auto stop_view = SplitBy(input, route_delimiter);
        stop_list.insert(stop_list.end(), stop_view.begin(), stop_view.end());
    }

    void Process(TransportManager& manager) const override {
        return manager.AddBus(name, direction, stop_list);
    }

    string name;
    BusRouteDirection direction;
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
    const size_t request_count = ReadNumberOnLine<size_t>(in_stream);

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

vector<ResponseHolder> ProcessRequests(const array<vector<RequestHolder>,
                                       REQUESTS_TYPE_COUNT>& requests,
                                       TransportManager& manager) {
    vector<ResponseHolder> responses;
    for (const auto& bucket : requests) {
        for (const auto& request_holder : bucket) {
            if (request_holder->type == Request::Type::GET_BUS) {
                const auto& request = static_cast<const GetBusRequest&>(*request_holder);
                responses.push_back(request.Process(manager));
            } else {
                const auto& request = static_cast<const ModifyRequest&>(*request_holder);
                request.Process(manager);
            }
        }
    }
    return responses;
}

ostream& operator<< (ostream& os, const ResponseHolder& response) {
    os << response->object_name << ": " << response->message;
    return os;
}

void PrintResponses(const vector<ResponseHolder>& responses, ostream& stream = cout) {
    for (const auto& response : responses) {
        stream << response << endl;
    }
}

void TestSmoke() {
    std::istringstream is;
    const std::string input =
        "10\n"
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
    TransportManager manager;
    const auto requests_to_write = ReadRequests(DataBaseMode::WRITE, is);
    const auto requests_to_read = ReadRequests(DataBaseMode::READ, is);
    ProcessRequests(requests_to_write, manager);
    const auto responses = ProcessRequests(requests_to_read, manager);
    PrintResponses(responses);
}

void TestNotFoundResponse() {
    std::istringstream is;
    const std::string input =
        "10\n"
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
        "1\n"
        "Bus 750\n";
    is.str(input);
    TransportManager manager;
    const auto requests_to_write = ReadRequests(DataBaseMode::WRITE, is);
    const auto requests_to_read = ReadRequests(DataBaseMode::READ, is);
    ProcessRequests(requests_to_write, manager);
    const auto responses = ProcessRequests(requests_to_read, manager);
    PrintResponses(responses);

}

void TestSimple() {
    ASSERT(true)
}

void TestAll() {
    TestRunner test_runner;
    RUN_TEST(test_runner, TestSimple);
//    RUN_TEST(test_runner, TestNotFoundResponse);
    RUN_TEST(test_runner, TestSmoke);

}

int main() {
    TestAll();
//    const auto requests_to_write = ReadRequests(DataBaseMode::WRITE, std::cin);
//    const auto requests_to_read = ReadRequests(DataBaseMode::READ, std::cin);
//    ProcessRequests(requests_to_write);
    return 0;
}