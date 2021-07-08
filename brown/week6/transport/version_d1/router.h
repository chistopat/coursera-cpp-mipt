#pragma once

#include <unordered_map>
#include <memory>

#include "stop.h"

class Router {
public:
    Router() = default;

public:
    void AddDistance(const std::string& from, const std::string& to, int distance) {
        matrix_[from][to] = distance;
    }

    std::optional<int> GetDistance(const std::string& from, const std::string& to) const {
        std::optional<int> result;
        if (matrix_.count(from)) {
            if (matrix_.at(from).count(to)) {
                result = matrix_.at(from).at(to);
            }
        }
        return result;
    }
private:
    std::unordered_map<std::string, std::unordered_map<std::string, int>> matrix_;
};