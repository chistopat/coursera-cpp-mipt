#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>
using namespace std;

class Animal {
public:
    Animal(string n) : Name(n) {

    }
    const string Name;
};


class Dog : public Animal{
public:
    Dog(string n) : Animal(n) {

    }
    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};


struct A {
    A() {
        std::cout << "C";
    }
    ~A() {
        std::cout << "D";
    }
};

int main () {
    unique_ptr<A> a(new A[5]);
}