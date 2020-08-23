//
// Created by Egor Chistyakov on 8/22/20.
//

#include "node.h"

bool EmptyNode::Evaluate(const Date& date, const std::string& event) const {
    return true;
}

LogicalOperationNode::LogicalOperationNode(
        LogicalOperation lo,
        std::shared_ptr<Node> lhs,
        std::shared_ptr<Node> rhs) : _lhs(lhs), _rhs(rhs), _lo(lo) {
}

bool LogicalOperationNode::Evaluate(const Date& date, const std::string& event) const {
    if (_lo == LogicalOperation::And) {
        return _lhs->Evaluate(date, event) and _rhs->Evaluate(date, event);
    } else if (_lo == LogicalOperation::Or) {
        return _lhs->Evaluate(date, event) or _rhs->Evaluate(date, event);
    }
    return false;
}

DateComparisonNode::DateComparisonNode(Comparison cmp,
                                       const Date& date)
                                       : _date(date),
                                       _cmpType(cmp) {
}

bool DateComparisonNode::Evaluate(const Date& date, const std::string& event) const {
    switch (_cmpType) {
        case Comparison::Less:
            return date.ToVector() < _date.ToVector();
        case Comparison::LessOrEqual:
            return date.ToVector() <= _date.ToVector();
        case Comparison::Greater:
            return date.ToVector() > _date.ToVector();
        case Comparison::GreaterOrEqual:
            return date.ToVector() >= _date.ToVector();
        case Comparison::Equal:
            return date.ToVector() == _date.ToVector();
        case Comparison::NotEqual:
            return date.ToVector() != _date.ToVector();
    }
}

EventComparisonNode::EventComparisonNode(Comparison cmp,
                                         const std::string event)
                                         : _event(event),
                                         _cmpType(cmp) {
}

bool EventComparisonNode::Evaluate(const Date& date, const std::string& event) const {
    switch (_cmpType) {
        case Comparison::Less:
            return event < _event;
        case Comparison::LessOrEqual:
            return event <= _event;
        case Comparison::Greater:
            return event > _event;
        case Comparison::GreaterOrEqual:
            return event >= _event;
        case Comparison::Equal:
            return event == _event;
        case Comparison::NotEqual:
            return event != _event;
    }
}


