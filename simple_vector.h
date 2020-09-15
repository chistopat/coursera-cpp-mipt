#pragma once

#include <cstdlib>
#include "algorithm"

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector()
    : _size(0)
    , _capacity(0)
    , _data(nullptr)
    , _end(nullptr) {}

  explicit SimpleVector(size_t size)
  : _size(size)
  , _capacity(size) {
    _data = new T[size];
    _end = _data + size;
  }

  ~SimpleVector() {
    delete [] _data;
  }

  T& operator[](size_t index) {
    return _data[index];
  }

  T* begin() {
    return _data;
  }
  T* end() {
    return _end;
  }

  const T* begin() const{
    return _data;
  }

  const T* end() const{
    return _end;
  }

  const size_t Size() const {
    return _size;
  }

  const size_t Capacity() const {
    return _capacity;
  }

  void PushBack(const T& value) {
    if (_size == 0) {
      _data = new T[1];
      _capacity ++;
    } else if (_size == _capacity) {
      auto temp = new T[_size * 2];
      for (auto i = 0; i < _size; ++i) {
        temp[i] = _data[i];
      }
      delete [] _data;
      _data = temp;
      _capacity *= 2;
    }
    _data[_size] = value;
    _size ++;
    _end = _data + _size;
  }

private:
  T* _data;
  T* _end;
  size_t _size;
  size_t _capacity;
  // Добавьте поля для хранения данных вектора
};
