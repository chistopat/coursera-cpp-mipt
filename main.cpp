#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <future>
#include <functional>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
      : first(begin)
      , last(end)
      , size_(distance(first, last))
  {
  }

  Iterator begin() const {
      return first;
  }

  Iterator end() const {
      return last;
  }

  size_t size() const {
      return size_;
  }

private:
  Iterator first, last;
  size_t size_;
};

template <typename Iterator>
class Paginator {
private:
  vector<IteratorRange<Iterator>> pages;

public:
  Paginator(Iterator begin, Iterator end, size_t page_size) {
      for (size_t left = distance(begin, end); left > 0; ) {
          size_t current_page_size = min(page_size, left);
          Iterator current_page_end = next(begin, current_page_size);
          pages.push_back({begin, current_page_end});

          left -= current_page_size;
          begin = current_page_end;
      }
  }

  auto begin() const {
      return pages.begin();
  }

  auto end() const {
      return pages.end();
  }

  size_t size() const {
      return pages.size();
  }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

std::vector<std::string> SplitString(const std::string &data, char delimiter = ' ') {
    if (data.empty()) {
        throw std::invalid_argument("Unable to split empty string");
    }
    std::vector<std::string> result;
    std::istringstream is(data);
    std::string token;
    while (getline(is, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

struct Stats {
  map<string, int> word_frequences;

  Stats& operator += (const Stats& other) {
      for (const auto& item : other.word_frequences) {
          word_frequences[item.first] += item.second;
      }
      return *this;
  }
};
Stats ExploreWords (set<string> key_words, vector<string> words) {
    Stats result;
    for (const auto& word : words) {
        if (key_words.count(word)) {
            ++result.word_frequences[word];
        }
    }
    return result;
}
Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    for (const auto& word : SplitString(line)) {
        if (key_words.count(word)) {
            ++result.word_frequences[word];
        }
    }
    return result;
}

vector<std::string> SplitLines(istream& input) {
    vector<std::string> result;
    for (string line; getline(input, line); ) {
        result.push_back(line);
    }
    return result;
}

Stats ExploreKeyWordsSingleThread (const set<string>& key_words, istream& input) {
    Stats result;
    for (string line; getline(input, line); ) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

//Stats ExploreKeyWordsSingleThread (const set<string>& key_words, vector<string>&& input) {
//    Stats result;
//    for (auto& line : input) {
//        result += ExploreLine(key_words, line);
//    }
//    return result;
//}

//Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
//    Stats result;
//    vector<future<Stats>> futures;
//    auto lines = SplitLines(input);
//    for (auto page : Paginate(lines, 1)) {
//        futures.push_back(async(
//            [&key_words, &page]{return ExploreKeyWordsSingleThread(key_words, {page.begin(), page.end()});})
//            );
//    }
//
//    return result;
//}

vector<string> GetWords(vector<std::string>&& lines) {
    vector<string> result;
    for (auto& line : lines) {
       for (auto& word : SplitString(line)) {
           result.push_back(word);
       }
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    LOG_DURATION("Total") {
        Stats result;
        vector<string> words = GetWords(SplitLines(input));
        vector<future<Stats>> futures;
        for (auto page : Paginate(words, words.size() / 4)) {
            vector<string> temp = {page.begin(), page.end()};
            futures.push_back(async(ExploreWords, key_words, temp));
        }
        for (auto& f : futures) {
            result += f.get();
        }
        return result;
    }

//    return async(ExploreKeyWordsSingleThread, ref(key_words), ref(input)).get();
}



void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
        {"yangle", 6},
        {"rocks", 2},
        {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}
