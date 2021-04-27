//
// Created by Egor Chistyakov on 27.04.2021.
//

#ifndef YELLOW_INI_H
#define YELLOW_INI_H

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>

namespace Ini {
    using Section = std::unordered_map<std::string, std::string>;
    class Document {
    public:
      Section& AddSection(std::string name);
      const Section& GetSection(const std::string& name) const;
      size_t SectionCount() const;

    private:
      std::unordered_map<std::string, Section> sections;
    };

    Document Load(std::istream& input);
}
#endif //YELLOW_INI_H
