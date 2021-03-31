#include "profile.h"
#include "test_runner.h"

#include <iostream>
#include <list>
#include <map>

class Football {
public:
  void Add(int current, int previous) {
    if (!_sportsmens_map.count(previous)) {
      _sportsmens_map[current] =
          _sportsmens.insert(_sportsmens.end(), current);
    } else {
      _sportsmens_map[current] =
          _sportsmens.insert(_sportsmens_map[previous], current);
    }
  }
  void Print() const {
    for (const auto i : _sportsmens) {
      std::cout << i << std::endl;
    }
  }

private:
  using Position = std::list<int>::iterator;

private:
  std::list<int> _sportsmens;
  std::map<int, Position> _sportsmens_map;
};

int main() {
  size_t n = 0;
  std::cin >> n;
  Football football;
  for (size_t i = 0; i < n; ++i) {
    int current = 0, previous = 0;
    std::cin >> current >> previous;
    football.Add(current, previous);
  }
  football.Print();
  return 0;
}
