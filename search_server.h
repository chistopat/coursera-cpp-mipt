#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

class InvertedIndex {
public:
  void Add(const string& document);
  list<size_t> Lookup(const string& word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

private:
  map<string, list<size_t>> index;
  vector<string> docs;
};

class QuickIndex {
public:
  struct DocumentFrequency {
    std::map<std::size_t, size_t> docid_to_frequency;
    DocumentFrequency& operator+=(const DocumentFrequency& other) {
       for (auto [docid, freq] : other.docid_to_frequency) {
         docid_to_frequency[docid] += freq;
       }
       return *this;
    }
  };
public:
  void Add(const string& document);
  DocumentFrequency Lookup(const string& word) const;

private:
  map<string , DocumentFrequency> index_;
  size_t serial_ = 0;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  QuickIndex index;
};
