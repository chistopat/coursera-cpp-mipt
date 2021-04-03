#pragma once

#include "http_request.h"
#include <vector>
#include <string>
#include <string_view>
#include <map>
using namespace std;

class Stats {
public:
  Stats();
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;
private:
  const std::string kUnknownUri = "unknown";
  const std::string kUnknownMethod = "UNKNOWN";
  const std::vector<std::string> kAllowedMethods = {"GET", "POST", "PUT", "DELETE", kUnknownMethod};
  const std::vector<std::string> kAllowedUri = {"/", "/order", "/product", "/basket", "/help", kUnknownUri};

private:
  map<string_view, int> method_stats_;
  map<string_view, int> uri_stats_;
};

HttpRequest ParseRequest(string_view line);
vector <string_view> SplitInToWords(const string& s);
