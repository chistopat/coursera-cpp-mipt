#include <array>
#include <stdexcept>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0)
      : _size(a_size)
      , _capacity(N) {
    if (a_size > N) {
      throw invalid_argument("invalid argument");
    }
  }

  T& operator[](size_t index) {
    return _data[index];
  }
  const T& operator[](size_t index) const {
    return _data.at(index);
  }

  auto begin() {
      return _data.begin();
  }
  auto end() {
    return _data.begin() + _size;
  }
  auto begin() const {
    return _data.begin();
  }
  auto end() const {
      return _data.begin() + _size;
  }

  size_t Size() const {
      return _size;
  }
  size_t Capacity() const {
    return _capacity;
  }

  void PushBack(const T& value) {
    if (_size == _capacity) {
      throw overflow_error("overflow error");
    }
    _data[_size++] = value;
  }
  T PopBack() {
    if (!_size) {
      throw underflow_error("underflow error");
    }
    return _data[--_size];
  }

private:
  size_t _size;
  size_t _capacity;
  std::array<T, N> _data;
};
