#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <future>
#include <functional>
#include <mutex>


using namespace std;

template <typename T>
class Synchronized {
public:
  explicit Synchronized(T initial = T())
      : value(move(initial))
  {
  }

  struct Access {
    T& ref_to_value;
    lock_guard<mutex> guard;
  };

  Access GetAccess() {
    return {value, lock_guard(m)};
  }

private:
  T value;
  mutex m;
};


class QuickIndex {
public:
  void Add(string&& document);
  vector<std::pair<size_t,size_t>> Lookup(const string& word) const;
  size_t GetDocumentsCount() const;
private:
  unordered_map<string, vector<std::pair<size_t,size_t>>> index_;
  size_t serial_ = 0;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);
  void AddQueriesStreamSingle(istream& query_input,
                              ostream& search_results_output);

private:
  Synchronized<QuickIndex> index_;
  std::vector<std::future<void>> futures_;
};
