//
// Created by Egor Chistyakov on 27.04.2021.
//
#include "ini.h"

namespace Ini {
    Section& Document::AddSection(std::string name) {
        return sections.insert({name, {}}).first->second;
    }
    const Section& Document::GetSection(const std::string& name) const {
        return sections.at(name);
    };
    size_t Document::SectionCount() const {
        return sections.size();
    };

    Document Load(std::istream& input) {
        auto ist = std::istream_iterator<std::string>(input, '\n');
        std::istringstream line;
        while(std::getline(input, line, '\n')) {
            std::cout << line << std::endl;
        }
        return {};
    };
}
