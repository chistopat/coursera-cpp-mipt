#pragma once

#include <vector>
#include <sstream>

#include "base_request.h"

std::vector<RequestHolder> ReadRequests(std::istream& input);


std::vector<double> ProcessRequests(const std::vector<RequestHolder>& requests);
void PrintResponses(const std::vector<double>& responses, std::ostream& stream);