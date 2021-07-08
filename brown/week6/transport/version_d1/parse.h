#pragma once

using namespace std;

string_view Strip(string_view s) {
    while (!s.empty() && isspace(s.front())) {
        s.remove_prefix(1);
    }
    while (!s.empty() && isspace(s.back())) {
        s.remove_suffix(1);
    }
    return s;
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
        result.push_back(Strip(s.substr(0, pos)));
        s.remove_prefix(pos != string::npos ? pos + delimiter.length() : s.size());
    }
    return result;
}