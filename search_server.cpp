#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"
#include "parse.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <functional>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

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


void SearchServer::UpdateDocumentBase(istream& document_input) {
  QuickIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }
    std::swap(new_index, index_.GetAccess().ref_to_value);
}

void SearchServer::AddQueriesStreamSingle(
  istream& query_input, ostream& search_results_output
) {
  size_t n = index_.GetAccess().ref_to_value.GetDocumentsCount();
  std:vector<pair<size_t, size_t>> search_results(n);
  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);
    search_results.assign(n, {0,0});
    for (const auto& word : words) {
          for (auto& [docid, freq] : index_.GetAccess().ref_to_value.Lookup(word)) {
            search_results[docid].first = docid;
            search_results[docid].second += freq;
          }
    }
      partial_sort(
        search_results.begin(),
        Head(search_results, 5).end(),
        search_results.end(),
        [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
          int64_t lhs_docid = lhs.first;
          auto lhs_hit_count = lhs.second;
          int64_t rhs_docid = rhs.first;
          auto rhs_hit_count = rhs.second;
          return make_pair(lhs_hit_count, -lhs_docid)
                   > make_pair(rhs_hit_count, -rhs_docid);
        }
      );

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(search_results, 5)) {
      if (hitcount != 0) {
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
      }
    }
    search_results_output << endl;
  }
}
  void SearchServer::AddQueriesStream(istream &query_input,
                                      ostream &search_results_output) {
    futures_.push_back(
        async(&SearchServer::AddQueriesStreamSingle, this,
              ref(query_input), ref(search_results_output)));
  }


void QuickIndex::Add(string&& document) {
  for (const auto& word : SplitIntoWords(document)) {
      if (index_.count(word)) {
        if (index_.at(word).back().first == serial_) {
          ++index_.at(word).back().second;
        } else {
          index_.at(word).push_back({serial_, 1});
        }
      } else {
        index_[word].push_back({serial_, 1});
      }
  }
  ++serial_;
}

vector<std::pair<size_t,size_t>> QuickIndex::Lookup(const string& word) const {
  if (index_.count(word)) {
    return index_.at(word);
  }
  return {};
}

size_t QuickIndex::GetDocumentsCount() const {
    return serial_;
}
