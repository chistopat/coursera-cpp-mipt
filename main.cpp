#include <iostream>
#include "deque"
#include "map"

using namespace std;

enum class OperandType {
    Addition,
    Subtraction,
    Multiplication,
    Division,
};

enum class ParensType{
    LRoundParen,
    RRoundParen
};

map<ParensType, const string> parensMap = {
        {ParensType::LRoundParen, "("},
        {ParensType::RRoundParen, ")"}
};

map<const string, OperandType> operandsMap = {
        {"+", OperandType::Addition},
        {"-", OperandType::Subtraction},
        {"*", OperandType::Multiplication},
        {"/", OperandType::Division, },
};

void MakeRoundParens(deque<string>& dq) {
    dq.push_front(parensMap[ParensType::LRoundParen]);
    dq.push_back(parensMap[ParensType::RRoundParen]);
}

void AddOperand(deque<string>& dq, string& operandStr, int value) {
    if (operandsMap.count(operandStr) == 1) {
        operandStr = " " + operandStr + " ";
        dq.push_back(operandStr);
        dq.push_back(to_string(value));
    }
}

int main() {
    int x;
    size_t n;
    cin >> x >> n;
    deque<string> dq;
    dq.push_back(to_string(x));
    while(n--) {
        string operand;
        int value;
        cin >> operand >> value;
        MakeRoundParens(dq);
        AddOperand(dq, operand, value);
    }
    for(const auto& item: dq) {
        cout<<item;
    }
    cout << endl;
    return 0;

}