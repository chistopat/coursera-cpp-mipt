#include "test_runner.h"

#include <cstddef>
using namespace std;

template <typename T>
class UniquePtr {
public:
  UniquePtr() : raw_pointer_(nullptr) {}
  explicit UniquePtr(T* ptr) : raw_pointer_(ptr) {}
  UniquePtr(const UniquePtr&) = delete;

  UniquePtr(UniquePtr&& other) : raw_pointer_(nullptr) {
      Reset(nullptr);
      Swap(other);
  }
  UniquePtr& operator = (const UniquePtr&) = delete;

  UniquePtr& operator = (nullptr_t null) {
      Reset(null);
      return *this;
  }
  UniquePtr& operator = (UniquePtr&& other) {
      Reset(nullptr);
      Swap(other);
      return *this;
  }

  ~UniquePtr() {
      delete raw_pointer_;
  }

  T& operator * () const {
      return *raw_pointer_;
  }

  T * operator -> () const {
      return raw_pointer_;
  }

  T * Release() {
      auto temp_ptr = raw_pointer_;
      raw_pointer_ = nullptr;
      return temp_ptr;
  }

  void Reset(T * ptr) {
      delete raw_pointer_;
      raw_pointer_ = ptr;
  }

  void Swap(UniquePtr& other) {
      if (this != &other) {
          std::swap(this->raw_pointer_, other.raw_pointer_);
      }
  }

  T* Get() const {
      return raw_pointer_;
  }

private:
  T* raw_pointer_;
};


struct Item {
  static int counter;
  int value;
  Item(int v = 0): value(v) {
      ++counter;
  }
  Item(const Item& other): value(other.value) {
      ++counter;
  }
  ~Item() {
      --counter;
  }
};

int Item::counter = 0;


void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);

    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}

void TestPtrConstructor() {
    UniquePtr<Item> ptr {new Item(42)} ;
    ASSERT_EQUAL(ptr.Get()->value, 42);
}

void TestPtrNullCtor() {
    UniquePtr<Item> ptr {new Item(42)} ;
    ptr = nullptr;
    ASSERT(!ptr.Get());
}

void TestPtrMoveCtor() {
    UniquePtr<Item> ptr1 {new Item(42)} ;
    UniquePtr<Item> ptr2 {move(ptr1)} ;
    ASSERT_EQUAL(ptr2.Get()->value, 42);
    ASSERT(!ptr1.Get());
}

void TestPtrMove() {
    UniquePtr<Item> ptr1 {new Item(42)} ;
    UniquePtr<Item> ptr2 = move(ptr1) ;
    ASSERT_EQUAL(ptr2.Get()->value, 42);
    ASSERT(!ptr1.Get());
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
    RUN_TEST(tr, TestPtrConstructor);
    RUN_TEST(tr, TestPtrNullCtor);
    RUN_TEST(tr, TestPtrMoveCtor);
    RUN_TEST(tr, TestPtrMove);
}
