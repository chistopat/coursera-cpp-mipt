#include <iostream>
#include "vector"
#include "string"
#include "sstream"
#include "test_runner.h"

std::vector<std::string> SplitIntoWords(const std::string& s){
    std::stringstream stream(s);
    std::string word;
    std::vector<std::string> result;
    while (getline(stream, word, ' ')) {
        result.push_back(word);
    }
    return result;
}

int main() {
    std::string s = "C Cpp Java Python";

    std::vector<std::string> words = SplitIntoWords(s);
    std::cout << words.size() << " ";
    for (auto it = begin(words); it != end(words); ++it) {
        if (it != begin(words)) {
            std::cout << "/";
        }
        std::cout << *it;
    }
    std::cout << std::endl;

    return 0;
}

