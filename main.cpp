#include <string>
#include <vector>
#include "deque"

using namespace std;


template<typename T>
class Deque {
public:
    Deque(size_t count=0) : _size(count) {;
    }
    bool Empty() const{
        return lhs.empty() and rhs.empty();
    }

    size_t Size() const {
        return lhs.size() + rhs.size();
    }
    T& operator[](size_t index) {

    }
    const T& operator[](size_t index) const{
        CheckIndex(index);

    }
    T& At(size_t index) {
        CheckIndex(index);

    }
    const T& At(size_t index) const;
    T& Front() {
        return *lhs.rbegin();
    }
    const T& Front() const {
        const auto result =  *lhs.rbegin();
        return result;
    }
    T& Back() {
        return *rhs.rbegin();
    }
    const T& Back() const {
        const auto result = *rhs.rbegin();
        return result;
    }
    void PushFront(T arg) {
        _size++;
        lhs.push_back(arg);
    }
    void PushBack(T arg){
        _size++;
        rhs.push_back(arg);
    }

private:
    size_t _size;
    vector<T> rhs;
    vector<T> lhs;

    void CheckIndex(size_t index) const{
        if (index > _size-1) {
            throw range_error("index out of range");
        }
    }
};


int main() {
    deque<int> d ;
    d[1];
}