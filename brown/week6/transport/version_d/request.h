#pragma once

#include "response.h"
#include "parse.h"
#include "segment.h"
#include "distance_map.h"
#include "json.h"

#include <algorithm>
using namespace std;

struct Request;
using RequestHolder = unique_ptr<Request>;

static constexpr auto kName = "name";
static constexpr auto kLatitude = "latitude";
static constexpr auto kLongitude = "longitude";
static constexpr auto kRoadDistances = "road_distances";
static constexpr auto kIsRoundtrip = "is_roundtrip";
static constexpr auto kStops = "stops";
static constexpr auto kId = "id";

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
    virtual void FromJson(Json::Node& data) = 0;
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


struct AddStopRequest : public ModifyRequest {
    AddStopRequest() : ModifyRequest(Type::ADD_STOP) {}
    void ParseFrom(string_view input) override {
        name = Strip(ReadToken(input, ":"));
        latitude = stod(string(ReadToken(input, ",")));
        longitude = stod(string(ReadToken(input, ",")));
        distance_map = DistanceMap::FromString(input);
    }

    void FromJson(Json::Node& data) override {
        name = data.AsMap().at(kName).AsString();
        latitude = data.AsMap().at(kLatitude).AsDouble();
        longitude = data.AsMap().at(kLongitude).AsDouble();
        distance_map = DistanceMap(data.AsMap().at(kRoadDistances).AsMap());
    }

    void Process(TransportManager& manager) const override {
        return manager.AddStop(name, latitude, longitude, distance_map);
    }

    string name;
    double latitude;
    double longitude;
    DistanceMap distance_map;
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

    void FromJson(Json::Node& data) override {
        name = data.AsMap().at(kName).AsString();
        is_roundtrip = data.AsMap().at(kIsRoundtrip).AsBool();
        for (const auto& stop : data.AsMap().at(kStops).AsArray()) {
            stop_list.push_back(stop.AsString());
        }
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


    void FromJson(Json::Node& data) override {
        name = data.AsMap().at(kName).AsString();
        id = data.AsMap().at(kId).AsInt();
    }
    ResponseHolder Process(const TransportManager& manager) const override {
        const auto bus_route = manager.GetBus(name);
        if (bus_route != nullptr) {
            auto response = BaseResponse::Create(BaseResponse::Type::GET_BUS);
            response->FromJson(id, "", bus_route->ToJson());
            return response;
        } else {
            auto response = BaseResponse::Create(BaseResponse::Type::ERROR);
            response->FromJson(id, "not found");
            return response;
        }
    }
    string name;
    unsigned id;
};

struct GetStopRequest : public ReadRequest<ResponseHolder> {
    GetStopRequest() : ReadRequest<ResponseHolder>(Type::GET_STOP) {}
    void ParseFrom(string_view input) override {
        name = Strip(input);
    }

    void FromJson(Json::Node& data) override {
        name = data.AsMap().at(kName).AsString();
        id = data.AsMap().at(kId).AsInt();
    }

    ResponseHolder Process(const TransportManager& manager) const override {
        const auto stop = manager.GetStop(name);
        if (stop.has_value()) {
            auto response = BaseResponse::Create(BaseResponse::Type::GET_STOP);
            response->FromJson(id, "", stop->ToJson());
            return response;
        } else {
            auto response = BaseResponse::Create(BaseResponse::Type::ERROR);
            response->FromJson(id, "not found");
            return response;
        }
    }

    string name;
    uint64_t id;
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