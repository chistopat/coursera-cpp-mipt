#include <iostream>
#include "test_runner.h"
#include "vector"
#include "string"
#include "sstream"
#include "forward_list"

using namespace std;

vector<string> SplitIntoWords(const string& s);

vector<string> SplitIntoWords(const string& s) {
    stringstream stream(s);
    vector<string> result;
    string token;
    while(getline(stream, token, ' ')) {
        result.push_back(token);
    }
    return result;
}

int main() {
    string s = "C Cpp Java Python";

    vector<string> words = SplitIntoWords(s);
    cout << words.size() << " ";
    for (auto it = begin(words); it != end(words); ++it) {
        if (it != begin(words)) {
            cout << "/";
        }
        cout << *it;
    }
    cout << endl;
    return 0;
}


