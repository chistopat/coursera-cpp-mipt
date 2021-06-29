#pragma once

#include <variant>
using namespace std;

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

RequestHolder ParseRequest(DataBaseMode mode, string_view request_str) {
    const auto request_type = ConvertRequestTypeFromString(Strip(ReadToken(request_str)), mode);
    if (!request_type) {
        return nullptr;
    }

    RequestHolder request = Request::Create(*request_type);
    if (request != nullptr) {
        request->ParseFrom(request_str);
    }

    return request;
}

using RequestGrid = array<vector<RequestHolder>, REQUESTS_TYPE_COUNT>;
using RequestList = vector<RequestHolder>;
variant<RequestList, RequestGrid> ReadRequests(DataBaseMode mode,
                                               ostringstream& log,
                                               istream& in_stream = cin,
                                               bool debug = false,
                                               string deadb33f = "") {
    const auto request_count = ReadNumberOnLine<size_t>(in_stream);

    log << request_count << '\n';

    RequestList request_list;
    RequestGrid request_grid;
    for (auto& bucket : request_grid) {
        bucket.reserve(request_count);
    }

    for (size_t i = 0; i < request_count; ++i) {
        string request_str;
        getline(in_stream, request_str);
        log << request_str << "\n";
        if (auto request = ParseRequest(mode, request_str)) {
            if (mode == DataBaseMode::READ) {
                request_list.push_back(move(request));
            } else {
                request_grid[static_cast<size_t>(request->type)].push_back(move(request));

            }
        }
    }
    if (const auto log_str = log.str();
        mode == DataBaseMode::READ and debug and log_str.find(deadb33f) != string::npos) {
        cerr << log_str << '\n';
        throw runtime_error(log_str);
    }
    if (mode == DataBaseMode::READ) {
        return request_list;
    }
    return request_grid;
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
    for (const auto& response : responses) {
        stream << response << endl;
    }
}
