//
// Created by chistopat on 09.05.2021.
//

#include "Common.h"

#include <sstream>

namespace Expressions {
    class Int : public Expression {
    public:
        explicit Int(int value)
            : value_(std::move(value)) {
        }

        std::string ToString() const override {
            std::ostringstream output;
            output << value_;
            return output.str();
        }

        int Evaluate() const override { return value_; }

    private:
        int value_;
    };

    class Sum : public Expression {
    public:
        const char kOperationSign = '+';
    public:
        explicit Sum(ExpressionPtr left, ExpressionPtr right)
            : left_(std::move(left))
            , right_(std::move(right)){

        }

        std::string ToString() const override {
            std::ostringstream output;
            output << '(' << left_->ToString() << ')'
                   << Sum::kOperationSign
                   <<  '(' << right_->ToString() << ')';
            return output.str();
        }

        int Evaluate() const override {
            return left_->Evaluate() + right_->Evaluate();
        }

    private:
        ExpressionPtr left_;
        ExpressionPtr right_;
        Expression* parent_ = nullptr;


    };

    class Product : public Expression {
    public:
        const char kOperationSign = '*';
    public:
        explicit Product(ExpressionPtr left, ExpressionPtr right)
            : left_(std::move(left))
            , right_(std::move(right)){

        }

        std::string ToString() const override {
            std::ostringstream output;
            output << '(' << left_->ToString() << ')'
                   << Product::kOperationSign
                   <<  '(' << right_->ToString() << ')';
            return output.str();
        }

        int Evaluate() const override {
            return left_->Evaluate() * right_->Evaluate();
        }

    private:
        ExpressionPtr left_;
        ExpressionPtr right_;
        Expression* parent_ = nullptr;
    };


}


ExpressionPtr Value(int value) {
    return std::make_unique<Expressions::Int>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<Expressions::Sum>(std::move(left), std::move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<Expressions::Product>(std::move(left), std::move(right));
}
