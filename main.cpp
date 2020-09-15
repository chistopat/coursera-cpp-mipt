#include "test_runner.h"

#include <vector>
using namespace std;

template <typename T>
class LinkedList {
public:

  struct Node {
    T value;
    Node* next = nullptr;
  };

  ~LinkedList() {
    while(headPtr) {
      this->PopFront();
    }
  }

  void PushFront(const T& value) {
    Node* newNode = new Node;
    newNode->value = value;
    newNode->next = headPtr;
    headPtr = newNode;
  }

  void InsertAfter(Node* node, const T& value) {
    if (node == nullptr) {
      this->PushFront(value);
      return void();
    }
    Node* newNode = new Node;
    newNode->value = value;
    newNode->next = node->next;
    node->next = newNode;
  }

  void RemoveAfter(Node* node) {
    if (node == nullptr) {
      this->PopFront();
      return void();
    }
    if (node->next == nullptr) {
      return void();
    }
    auto curr = node->next;
    node->next = curr->next;
    delete curr;
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

private:
  Node* headPtr = nullptr;
};

template <typename T>
vector<T> ToVector(const LinkedList<T>& list) {
  vector<T> result;
  for (auto node = list.GetHead(); node; node = node->next) {
    result.push_back(node->value);
  }
  return result;
}

void TestPushFront() {
  LinkedList<int> list;

  list.PushFront(1);
  ASSERT_EQUAL(list.GetHead()->value, 1);
  list.PushFront(2);
  ASSERT_EQUAL(list.GetHead()->value, 2);
  list.PushFront(3);
  ASSERT_EQUAL(list.GetHead()->value, 3);

  const vector<int> expected = {3, 2, 1};
  ASSERT_EQUAL(ToVector(list), expected);
}

void TestInsertAfter() {
  LinkedList<string> list;

  list.PushFront("a");
  auto head = list.GetHead();
  ASSERT(head);
  ASSERT_EQUAL(head->value, "a");

  list.InsertAfter(head, "b");
  const vector<string> expected1 = {"a", "b"};
  ASSERT_EQUAL(ToVector(list), expected1);

  list.InsertAfter(head, "c");
  const vector<string> expected2 = {"a", "c", "b"};
  ASSERT_EQUAL(ToVector(list), expected2);
}

void TestRemoveAfter() {
  LinkedList<int> list;
  for (int i = 1; i <= 5; ++i) {
    list.PushFront(i);
  }

  const vector<int> expected = {5, 4, 3, 2, 1};
  ASSERT_EQUAL(ToVector(list), expected);

  auto next_to_head = list.GetHead()->next;
  list.RemoveAfter(next_to_head); // удаляем 3
  list.RemoveAfter(next_to_head); // удаляем 2

  const vector<int> expected1 = {5, 4, 1};
  ASSERT_EQUAL(ToVector(list), expected1);

  while (list.GetHead()->next) {
    list.RemoveAfter(list.GetHead());
  }
  ASSERT_EQUAL(list.GetHead()->value, 5);
}

void TestPopFront() {
  LinkedList<int> list;

  for (int i = 1; i <= 5; ++i) {
    list.PushFront(i);
  }
  for (int i = 1; i <= 5; ++i) {
    list.PopFront();
  }
  ASSERT(list.GetHead() == nullptr);
}

void MyTest() {
  LinkedList<int> list;
  list.PopFront();
  list.PushFront(1);
  list.PopFront();
  list.PopFront();
  list.RemoveAfter(nullptr);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestPushFront);
  RUN_TEST(tr, TestInsertAfter);
  RUN_TEST(tr, TestRemoveAfter);
  RUN_TEST(tr, TestPopFront);
  RUN_TEST(tr, MyTest);
  return 0;
}
