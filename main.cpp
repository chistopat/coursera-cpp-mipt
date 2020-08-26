#include <iostream>
#include "deque"
#include "map"
#include "sstream"

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

typedef size_t TPriority;

map<ParensType, string> parensMap = {
        {ParensType::LRoundParen, "("},
        {ParensType::RRoundParen, ")"}
};

map<string, pair<OperandType, TPriority>> operandsMap = {
        {"+", make_pair(OperandType::Addition, 1)},
        {"-", make_pair(OperandType::Subtraction, 1)},
        {"*", make_pair(OperandType::Multiplication, 2)},
        {"/", make_pair(OperandType::Division, 2)},
};

class Expression {
public:
    explicit Expression(int init_value) : _last_priority(0) {
        _dq.push_back(to_string(init_value));
    }
    void AddOperand(string& operandStr, int value) {
        if (operandsMap.count(operandStr) == 0) {
            return void();
        }
        auto [operand, priority] = operandsMap[operandStr];
        if (priority > _last_priority && _last_priority != 0) {
            MakeRoundParens();
        }
        operandStr = " " + operandStr + " ";
        _dq.push_back(operandStr);
        _dq.push_back(to_string(value));
        _last_priority = priority;

    }
    [[nodiscard]] string ToSting() const {
        stringstream stream;
        for (const auto& item : _dq) {
            stream << item;
        }
        return stream.str();
    }
private:
    deque<string> _dq;
    TPriority _last_priority;
    void MakeRoundParens() {
        _dq.push_front(parensMap[ParensType::LRoundParen]);
        _dq.push_back(parensMap[ParensType::RRoundParen]);
    }
};





int main() {
    int value;
    size_t n;
    cin >> value >> n;
    Expression expression(value);
    while(n--) {
        string operand;
        cin >> operand >> value;
        expression.AddOperand(operand, value);
    }
    cout << expression.ToSting()<< endl;
    return 0;

}