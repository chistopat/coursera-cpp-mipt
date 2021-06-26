#pragma once

#include <memory>
#include <string_view>
#include <unordered_map>

#include "response.h"

struct BaseRequest;
using RequestHolder = std::unique_ptr<BaseRequest>;

struct BaseRequest {
    enum class Type {
        GET_BUS,
        ADD_BUS,
        ADD_STOP,
    };
    BaseRequest(Type type) : type(type) {}
    static RequestHolder Create(Type type);
    virtual void ParseFrom(std::string_view input) = 0;
    virtual ~BaseRequest() = default;

    const Type type;
};

template <typename ResultType>
struct ReadRequest : public BaseRequest {
    enum class Type {
       GET_BUS,
    };
    using BaseRequest::BaseRequest;
    virtual ResultType Process(const BudgetManager& manager) const = 0;
};

struct ModifyRequest : public BaseRequest {
    enum class Type {
        ADD_BUS,
        ADD_STOP,
    };
    using BaseRequest::BaseRequest;
    virtual void Process(BudgetManager& manager) const = 0;
};

const std::unordered_map<std::string_view, ReadRequest<BaseResponse>::Type> STR_TO_READ_REQUEST_TYPE = {
    {"Bus", ReadRequest<BaseResponse>::Type::GET_BUS},
};

const std::unordered_map<std::string_view, ModifyRequest::Type> STR_TO_READ_MODIFY_TYPE = {
    {"Bus", ModifyRequest::Type::ADD_BUS},
    {"Stop", ModifyRequest::Type::ADD_STOP},
};