#pragma once

#include <variant>
#include "json.h"

using namespace std;

static constexpr auto kBaseRequests = "base_requests";
static constexpr auto kStatRequests = "stat_requests";
static constexpr auto kRequestType = "type";


bool AreDoubleSame(double left, double right)
{
    return std::fabs(left - right) < 1e-5;
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

RequestHolder ParseRequest(Json::Node& request_json, Request::Type request_type) {
    RequestHolder request = Request::Create(request_type);
    if (request != nullptr) {
        request->FromJson(request_json);
    }

    return request;
}

using RequestGrid = array<vector<RequestHolder>, REQUESTS_TYPE_COUNT>;
using RequestList = vector<RequestHolder>;

struct RequestsContainer {
    RequestGrid base_requests;
    RequestList stat_requests;
};

RequestsContainer ReadRequests(istream& in_stream = cin, bool debug = false, string token = "") {
//    const string input{istream_iterator<char>(in_stream), istream_iterator<char>()};
//    if (debug) {
//        if (input.find(token) != string::npos) {
//            throw runtime_error(input);
//        }
//    }
    const auto data = Json::Load(in_stream);

    RequestsContainer result;

    std::set<uint64_t> uniq_ids;

    for (auto request : data.GetRoot().AsMap().at(kBaseRequests).AsArray()) {
        const string request_type_str = request.AsMap().at(kRequestType).AsString();
        const auto request_type = ConvertRequestTypeFromString(request_type_str, DataBaseMode::WRITE);
        result.base_requests[static_cast<size_t>(*request_type)].push_back(ParseRequest(request, *request_type));
    }

    for (auto request : data.GetRoot().AsMap().at(kStatRequests).AsArray()) {
        const string request_type_str =  request.AsMap().at(kRequestType).AsString();
        const auto request_type = ConvertRequestTypeFromString(request_type_str, DataBaseMode::READ);
        result.stat_requests.push_back(ParseRequest(request, *request_type));
    }

    return result;
}

vector<ResponseHolder> ProcessRequests(const RequestsContainer& requests,
                                           TransportManager& manager) {
    vector<ResponseHolder> responses;
    for (const auto& bucket : requests.base_requests) {
        for (const auto& request_holder : bucket) {
            const auto& request = dynamic_cast<const ModifyRequest&>(*request_holder);
            request.Process(manager);
        }
    }
    for (const auto& request_holder : requests.stat_requests) {
        const auto& request = dynamic_cast<const ReadRequest<ResponseHolder>&>(*request_holder);
        responses.push_back(request.Process(manager));
    }
    return responses;
}

vector<ResponseHolder> ProcessReadRequests(const variant<RequestList, RequestGrid>& requests,
                                           TransportManager& manager) {
    vector<ResponseHolder> responses;
    for (const auto& request_holder : get<RequestList>(requests)) {
        const auto& request = dynamic_cast<const ReadRequest<ResponseHolder>&>(*request_holder);
        responses.push_back(request.Process(manager));
    }
    return responses;
}

void ProcessWriteRequests(const variant<RequestList, RequestGrid>& requests,
                          TransportManager& manager) {
    for (const auto& bucket : get<RequestGrid>(requests)) {
        for (const auto& request_holder : bucket) {
            const auto& request = dynamic_cast<const ModifyRequest&>(*request_holder);
            request.Process(manager);
        }
    }
}

void PrintResponses(const vector<ResponseHolder>& responses, ostream& stream = cout) {
    bool first = true;
    stream << "[";
    for (const auto& response : responses) {
        if (!first) {
            stream << ", ";
        }
        first = false;
        stream << response->ToString();
    }
    stream << "]" << endl;
}