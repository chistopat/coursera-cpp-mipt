//
// Created by chistopat on 27.04.2021.
//

#ifndef YELLOW_BROWN_WEEK2_INI_PARSER_INI_H_
#define YELLOW_BROWN_WEEK2_INI_PARSER_INI_H_

#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;
namespace Ini {
    using Section = unordered_map<string, string>;

    class Document {
    public:
        Section& AddSection(string name);
        const Section& GetSection(const string& name) const;
        size_t SectionCount() const;

    private:
        unordered_map<string, Section> sections;
    };

    Document Load(istream& input);
}

#endif // YELLOW_BROWN_WEEK2_INI_PARSER_INI_H_
