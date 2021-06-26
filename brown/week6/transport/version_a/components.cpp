//
// Created by chistopat on 26.06.2021.
//
#include <iostream>

#include "components.h"
#include "parse.h"

using namespace std;

optional<Request::Type> ConvertRequestTypeFromString(string_view type_str) {
    if (const auto it = STR_TO_REQUEST_TYPE.find(type_str);
        it != STR_TO_REQUEST_TYPE.end()) {
        return it->second;
    } else {
        return nullopt;
    }
}

RequestHolder ParseRequest(string_view request_str) {
    const auto request_type = ConvertRequestTypeFromString(ReadToken(request_str));
    if (!request_type) {
        return nullptr;
    }
    RequestHolder request = Request::Create(*request_type);
    if (request) {
        request->ParseFrom(request_str);
    };
    return request;
}

vector<RequestHolder> ReadRequests(istream& in_stream = cin) {
    const size_t request_count = ReadNumberOnLine<size_t>(in_stream);

    vector<RequestHolder> requests;
    requests.reserve(request_count);

    for (size_t i = 0; i < request_count; ++i) {
        string request_str;
        getline(in_stream, request_str);
        if (auto request = ParseRequest(request_str)) {
            requests.push_back(move(request));
        }
    }
    return requests;
}

vector<double> ProcessRequests(const vector<RequestHolder>& requests) {
    vector<double> responses;
    BudgetManager manager;
    for (const auto& request_holder : requests) {
        if (request_holder->type == Request::Type::COMPUTE_INCOME) {
            const auto& request = static_cast<const ComputeIncomeRequest&>(*request_holder);
            responses.push_back(request.Process(manager));
        } else {
            const auto& request = static_cast<const ModifyRequest&>(*request_holder);
            request.Process(manager);
        }
    }
    return responses;
}

void PrintResponses(const vector<double>& responses, ostream& stream = cout) {
    for (const double response : responses) {
        stream << response << endl;
    }
}
