#include <iostream>
#include "vector"

using namespace std;

int main() {
    int64_t n;
    cin >> n;
    if (n == 0) {
        return 0;
    }
    vector<int64_t> temperature(n);
    int64_t sum = 0;
    for(auto& item : temperature) {
        cin >> item;
        sum += item;
    }
    int64_t tempLen = temperature.size();
    int64_t avg = sum / tempLen;
    vector<int64_t> upperAverageIndexes;
    for (int64_t i = 0; i<tempLen; ++i) {
        if(temperature[i]>avg) {
            upperAverageIndexes.push_back(i);
        }
    }
    cout << upperAverageIndexes.size() << endl;
    for (auto& i : upperAverageIndexes) {
        cout << i << ' ';
    }
    return 0;
}
