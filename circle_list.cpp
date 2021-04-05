//
// Created by Egor Chistyakov on 05.04.2021.
//

#include "cinttypes"
#include "vector"
#include "iostream"

using namespace std;

template <typename T>
class CircleList {
public:
  struct Node {
    T value;
    Node* next = nullptr;
  };

public:
  template<typename RandomIt>
  CircleList(RandomIt first, RandomIt last) {
      auto head = prev(last);
      while(first != last && --last >= first) {
          Insert(*last);
      }
  }

  ~CircleList() {
      while(headPtr) {
          this->PopFront();
      }
  }

  void Insert(const T& value) {
      Node* newNode = new Node;
      newNode->value = value;
      if (size == 0) {
          headPtr = newNode;
          newNode->next = headPtr;
      } else if (size == 1) {
          newNode->next = headPtr;
      }

      ++size;
  }

  void PopFront() {
      if(headPtr == nullptr) {
          return void();
      } else if (headPtr->next == nullptr){
          delete headPtr;
          headPtr = nullptr;
      } else {
          auto temp = headPtr;
          headPtr = headPtr->next;
          delete temp;
      }
  }

  Node* GetHead() { return headPtr; }
  const Node* GetHead() const { return headPtr; }

public:
  uint32_t size = 0;

private:
  Node* headPtr = nullptr;
};

template <typename T>
vector<T> ToVector(const CircleList<T>& list, size_t n) {
    vector<T> result;
    for (auto node = list.GetHead(); n; n--) {
        result.push_back(node->value);
    }
    return result;
}


int main() {
    {
        vector<int> v = {1, 2, 3, 4, 5};
        CircleList<int> cl(v.begin(), v.end());
        for (auto i : ToVector(cl, 5)) {
            std::cout << i << std::endl;
        }
        std::cout << std::endl;

    }
    {
        vector<int> v = {};
        CircleList<int> cl(v.begin(), v.end());
        for (auto i : ToVector(cl, 5)) {
            std::cout << i << std::endl;
        }
        std::cout << std::endl;

    }
    {
        vector<int> v = {1, 2, 3, 4, 5};
        CircleList<int> cl(v.begin(), v.end());
        for (auto i : ToVector(cl, 10)) {
            std::cout << i << std::endl;
        }
        std::cout << std::endl;

    }

}
