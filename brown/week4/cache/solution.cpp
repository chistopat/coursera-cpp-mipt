#include "Common.h"

#include <list>
#include <unordered_map>
#include <algorithm>
#include <atomic>
#include <mutex>

using namespace std;

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  )
  : books_unpacker_(std::move(books_unpacker))
  , used_size_(0)
  , settings_(settings) {
  }

  BookPtr GetBook(const string& book_name) override {
      lock_guard<mutex> lock(mutex_);
      auto it = name_to_iterator_.find(book_name);
      if (it != name_to_iterator_.end()) {
          books_chache_.splice(books_chache_.end(), books_chache_, it->second);
          return *it->second;
      }

      BookPtr book = books_unpacker_->UnpackBook(book_name);

      if (book->GetContent().length() > settings_.max_memory) {
          books_chache_.clear();
          name_to_iterator_.clear();
          used_size_ = 0;
      } else {
          name_to_iterator_[book->GetName()] = books_chache_.insert(books_chache_.end(), book);
          used_size_ += book->GetContent().length();

          while (used_size_ > settings_.max_memory) {
              used_size_ -= books_chache_.front()->GetContent().length();
              name_to_iterator_.erase(books_chache_.front()->GetName());
              books_chache_.pop_front();
          }
      }
      return book;
  }


private:
  std::mutex mutex_;
  std::list<BookPtr> books_chache_;
  std::unordered_map<std::string, std::list<BookPtr>::iterator> name_to_iterator_;
  shared_ptr<IBooksUnpacker> books_unpacker_;
  std::atomic_size_t used_size_;
  Settings settings_;

};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
    return std::make_unique<LruCache>(move(books_unpacker), settings);
}
