#pragma once

#include "json.h"
using namespace nlohmann;
using namespace std;
class BaseResponse;
using ResponseHolder = unique_ptr<BaseResponse>;

class BaseResponse {
public:
    enum class Type {
        GET_BUS,
        GET_STOP,
        ERROR,
        LAST_,
    };
public:
    static ResponseHolder Create(BaseResponse::Type type);
    BaseResponse(BaseResponse::Type type) : type(type) {}
    virtual std::string ToString() = 0;
    virtual void FromJson(uint64_t request_id, string msg, json&& data) = 0;
    virtual ~BaseResponse() = default;

    BaseResponse::Type type;
    json json_data;
};


class ErrorResponse : public BaseResponse {
public:
    using BaseResponse::BaseResponse;
    ErrorResponse() : BaseResponse(BaseResponse::Type::ERROR) {}
    std::string ToString() override{
        return json_data.dump();
    }
    void FromJson(uint64_t request_id, string msg, json&& data) override{
        json_data["request_id"] = request_id;
        json_data["error_message"] = move(msg);
    }
};

class GetStopResponse : public BaseResponse {
public:
    using BaseResponse::BaseResponse;
    GetStopResponse() : BaseResponse(BaseResponse::Type::GET_STOP) {}

    std::string ToString() override{
        return json_data.dump();
    }

    void FromJson(uint64_t request_id, string msg, json&& data) override {
        json_data = move(data);
        json_data["request_id"] = request_id;
    }
};



class GetBusResponse : public BaseResponse {
public:
    using BaseResponse::BaseResponse;
    GetBusResponse() : BaseResponse(BaseResponse::Type::GET_BUS) {}

    std::string ToString() override{
        return json_data.dump();
    }

    void FromJson(uint64_t request_id, string msg, json&& data) override {
        json_data = move(data);
        json_data["request_id"] = request_id;
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