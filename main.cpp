#include <iostream>
#include "map"
#include "string"

using namespace std;

template <typename TKey, typename TValue>
TValue& GetRefStrict(map<TKey, TValue>& m, const TKey& key);

template <typename TKey, typename TValue>
TValue& GetRefStrict(map<TKey, TValue>& m, const TKey& key) {
    if(m.count(key) == 0) {
        throw runtime_error("Key not found");
    }
    return m[key];
}

int main() {
    map<int, string> m = {{0, "value"}};
    string& item = GetRefStrict(m, 0);
    item = "newvalue";
    cout << m[0] << endl;
    return 0;
}
