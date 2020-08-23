#pragma once
#include "date.h"

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation {
    And,
    Or
};

class Node {
public:
    virtual bool Evaluate(const Date&, const std::string& event) const = 0;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(Comparison cmp, const Date& date);
    bool Evaluate(const Date&, const std::string& event) const override;
private:
    Date _date;
    Comparison _cmpType;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(Comparison cmp, const std::string event);
    bool Evaluate(const Date&, const std::string& event) const override;
private:
    Comparison _cmpType;
    const std::string _event;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(LogicalOperation lo,
                         std::shared_ptr<Node> lhs,
                         std::shared_ptr<Node> rhs);
    bool Evaluate(const Date&, const std::string& event) const override;
private:
    std::shared_ptr<Node> _lhs;
    std::shared_ptr<Node> _rhs;
    LogicalOperation _lo;
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date&, const std::string& event) const override;
};