#pragma once

#include "vector"
#include "stdexcept"

template<typename T>
class Deque {
public:
  Deque() = default;

  bool Empty() const {
    return lhs.empty() && rhs.empty();
  }

  size_t Size() const {
    return lhs.size() + rhs.size();
  }

  const T& operator[](size_t index) const {
    if (index < lhs.size()) {
      return lhs[lhs.size() - 1 - index];
    }
    return rhs[index - lhs.size()];
  }

  T& operator[](size_t index) {
    if (index < lhs.size()) {
      return lhs[lhs.size() - 1 - index];
    }
    return rhs[index - lhs.size()];
  }

  const T& At(size_t index) const {
    if(index >= Size()) {
      throw std::out_of_range("Out of range");
    }
    if (index < lhs.size()) {
      return lhs[lhs.size() - 1 - index];
    }
    return rhs[index - lhs.size()];
  }

  T& At(size_t index){
    if(index >= Size()) {
      throw std::out_of_range("Out of range");
    }
    if (index < lhs.size()) {
      return lhs[lhs.size() - 1 - index];
    }
    return rhs[index - lhs.size()];
  }

  T& Front(){
    if (lhs.empty()) {
      return *rhs.begin();
    }
    return *lhs.rbegin();
  }

  const T& Front() const {
    if (lhs.empty()) {
      return *rhs.begin();
    }
    return *lhs.rbegin();
  }

  T& Back() {
    if (rhs.empty()) {
      return *lhs.begin();
    }
    return *rhs.rbegin();
  }

  const T& Back() const{
    if (rhs.empty()) {
      return *lhs.begin();
    }
    return *rhs.rbegin();
  }

  void PushFront(T arg) {
    lhs.push_back(arg);
  }

  void PushBack(T arg) {
    rhs.push_back(arg);
  }

private:
  std::vector<T> rhs;
  std::vector<T> lhs;
};



