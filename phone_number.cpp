//
// Created by Egor Chistyakov on 8/1/20.
//

#include "phone_number.h"
#include <limits>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string>
SplitString(const std::string &row, char delimiter) {
    std::stringstream stream(row);
    std::vector<std::string> tokens;
    stream.ignore(1);
    std::string item;
    while (getline(stream, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

std::string
JoinStrings(const std::vector<std::string> &tokens, char delimiter, size_t indent) {
    std::stringstream stream;
    bool first = true;
    for(size_t i = indent; i<tokens.size(); i++) {
        if(!first) {
            stream << delimiter;
        }
        first = false;
        stream << tokens[i];
    }
    return stream.str();
}

PhoneNumber::PhoneNumber(const std::string &international_number) {
  auto tokens = SplitString(international_number, '-');
  if (international_number.empty() ||
      international_number[0] != '+' ||
      tokens.size() < 3) {
      throw std::invalid_argument("Invalid phone");
  }
  country_code_ = tokens[0];
  city_code_ = tokens[1];
  local_number_ = JoinStrings(tokens, '-', 2);
}

std::string PhoneNumber::GetCountryCode() const {
    return country_code_;
}
std::string PhoneNumber::GetCityCode() const {
    return city_code_;
}
std::string PhoneNumber::GetLocalNumber() const {
    return local_number_;
}
std::string PhoneNumber::GetInternationalNumber() const {
    std::ostringstream stream;
    stream << '+' << country_code_ << '-' << city_code_
           << '-' << local_number_;
    return  stream.str();
}
