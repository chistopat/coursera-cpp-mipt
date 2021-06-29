#pragma once

using namespace std;

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

ostream& operator<< (ostream& os, const ResponseHolder& response) {
    os << response->object_type << " " << response->object_name << ": " << response->message;
    return os;
}