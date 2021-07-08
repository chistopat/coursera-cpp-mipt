#pragma once

#include "json.h"
using namespace std;
class BaseResponse;
using ResponseHolder = unique_ptr<BaseResponse>;

static constexpr auto kErrorMessage = "error_message";
static constexpr auto kRequestId = "request_id";

static constexpr auto kCurvature = "curvature";
static constexpr auto kRoadLength = "route_length";
static constexpr auto kStopCount = "stop_count";
static constexpr auto kUniqueStopCount = "unique_stop_count";
static constexpr auto kBuses = "buses";


class BaseResponse {
public:
    enum class Type {
        GET_BUS,
        GET_STOP,
        ERROR,
    };
public:
    static ResponseHolder Create(BaseResponse::Type type);
    BaseResponse(BaseResponse::Type type) : type(type) {}
    virtual std::string ToString() = 0;
    virtual void FromJson(int request_id, string msg, map<string, Json::Node>&& data= {}) = 0;
    virtual ~BaseResponse() = default;

    BaseResponse::Type type;
    map<string, Json::Node> json_data;
};


class ErrorResponse : public BaseResponse {
public:
    using BaseResponse::BaseResponse;
    ErrorResponse() : BaseResponse(BaseResponse::Type::ERROR) {}
    std::string ToString() override{
        ostringstream os;

        os << "{"
        << "\"" << kErrorMessage << "\": "
        << "\"" << json_data.at(kErrorMessage).AsString() << "\", "
        << "\"" << kRequestId << "\": "
           << json_data.at(kRequestId).AsInt() << "}";
        return os.str();
    }
    void FromJson(int request_id, string msg, map<string, Json::Node>&& data) override{
        json_data[kRequestId] = Json::Node(request_id);
        json_data[kErrorMessage] = Json::Node(msg);
    }
};

class GetStopResponse : public BaseResponse {
public:
    using BaseResponse::BaseResponse;
    GetStopResponse() : BaseResponse(BaseResponse::Type::GET_STOP) {}

    std::string ToString() override{
        ostringstream os;
        const auto buses = json_data.at(kBuses).AsArray();
        os << "{"
           << "\"" << kBuses << "\": " << "[";
        bool first = true;
        for (const auto& bus : buses) {
            if (!first) {
                os << ", ";
            }
            first = false;
            os << "\"" << bus.AsString() << "\"";
        }
        os << "], " << "\"" << kRequestId << "\": "
           <<  json_data.at(kRequestId).AsInt() << "}";

        return os.str();
    }

    void FromJson(int request_id, string msg, map<string, Json::Node>&& data) override {
        json_data = move(data);
        json_data[kRequestId] = Json::Node(request_id);
    }
};



class GetBusResponse : public BaseResponse {
public:
    using BaseResponse::BaseResponse;
    GetBusResponse() : BaseResponse(BaseResponse::Type::GET_BUS) {}

    std::string ToString() override{
        ostringstream os;
        os << "{"
           << "\"" << kCurvature << "\": "
           << json_data.at(kCurvature).AsDouble() << ", "
           << "\"" << kRequestId << "\": "
           << json_data.at(kRequestId).AsInt() << ", "
           << "\"" << kRoadLength << "\": "
           << json_data.at(kRoadLength).AsInt() << ", "
           << "\"" << kStopCount << "\": "
           << json_data.at(kStopCount).AsInt() << ", "
           << "\"" << kUniqueStopCount << "\": "
           << json_data.at(kUniqueStopCount).AsInt() << "}";

        return os.str();

    }

    void FromJson(int request_id, string msg, map<string, Json::Node>&& data) override {
        json_data = move(data);
        json_data[kRequestId] = Json::Node(request_id);
    }
};

ResponseHolder BaseResponse::Create(BaseResponse::Type type) {
    switch (type) {
        case BaseResponse::Type::GET_BUS:
            return make_unique<GetBusResponse>();
        case BaseResponse::Type::GET_STOP:
            return make_unique<GetStopResponse>();
        case BaseResponse::Type::ERROR:
            return make_unique<ErrorResponse>();
        default:
            return nullptr;
    }
}