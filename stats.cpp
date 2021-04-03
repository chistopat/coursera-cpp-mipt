//
// Created by Egor Chistyakov on 03.04.2021.
//
#include "stats.h"

std::vector<std::string_view>
splitSV(std::string_view strv, std::string_view delims = " ")
{
    std::vector<std::string_view> output;
    size_t first = 0;

    while (first < strv.size())
    {
        const auto second = strv.find_first_of(delims, first);

        if (first != second)
            output.emplace_back(strv.substr(first, second-first));

        if (second == std::string_view::npos)
            break;

        first = second + 1;
    }

    return output;
}

std::vector<std::string_view> SplitInToWords(const std::string_view& str) {
    std::vector<std::string_view > result;
    size_t pos = 0;
    const size_t pos_end = std::string_view::npos;
    while(true) {
        size_t space = str.find(' ', pos);
        result.push_back(
            space == pos_end ? str.substr(pos) : str.substr(pos , space - pos));
        if (space == pos_end) {
            break;
        } else {
            pos = space + 1;
        }
    }
    return result;
}

HttpRequest ParseRequest(string_view line) {
    std::vector<std::string_view> tokens = splitSV(line);
    return {tokens[0], tokens[1], tokens[2]};
}

Stats::Stats() {
    for (const auto& method : kAllowedMethods) {
        method_stats_[method] = 0;
    }

    for (const auto& uri : kAllowedUri) {
        uri_stats_[uri] = 0;
    }
}

const map<string_view, int>& Stats::GetMethodStats() const {
    return method_stats_;
}

const map<string_view, int>& Stats::GetUriStats() const {
    return uri_stats_;
}

void Stats::AddUri(string_view uri) {
    !uri_stats_.count(uri) ? ++uri_stats_[kUnknownUri] : ++uri_stats_[uri];
}

void Stats::AddMethod(string_view method) {
    !method_stats_.count(method) ? ++method_stats_[kUnknownMethod] : ++method_stats_[method];
}
