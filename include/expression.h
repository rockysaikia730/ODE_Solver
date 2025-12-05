#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <vector>
#include <cmath>
#include <memory>
#include "dynamic_tensor.h"

/**
 * @brief Abstract Base Class for Mathematical Expressions.
 * Supports evaluation of expressions involving time 't' and tensor 'y'.
 * Derived classes must implement the Evaluate method.
 */
class Expression {
public:
    /**
     * @brief Virtual Destructor.
     */
    virtual ~Expression() = default;

    /**
     * @brief Evaluate the expression at given time and tensor.
     *
     * @param t The time variable.
     * @param y The DynamicTensor variable.
     * @return The evaluated result as a double. MISSING IMPLEMENTATION FOR COMPLEX. ALWAYS REAL FOR NOW???
     */
    virtual double Evaluate(double t, const DynamicTensor& y) const = 0;
};

/** 
 * @brief Constant Expression Class.
 * Represents a constant numerical value in an expression.
 */
class ConstantExpression : public Expression {
public:
    ConstantExpression(double val) : value_(val) {}
    double Evaluate(double t, const DynamicTensor& y) const override {
        return value_;
}
private:
    double value_;
};

class TExpression : public Expression {
public:
    double Evaluate(double t, const DynamicTensor& y) const override {
        return t;
    }
};

/** 
 * @brief Y Expression Class.
 * Represents access to elements of the DynamicTensor 'y'.
 */
class YExpression : public Expression {
public:
    YExpression(const std::vector<size_t>& idx) : indices(idx) {}
    double Evaluate(double t, const DynamicTensor& y) const override {
        return y.at<double>(indices);
    }
private:
    std::vector<size_t> indices;
};

/** 
 * @brief Binary Expression Class.
 * Handles binary operations like +, -, *, /, ^.
 */
class BinaryExpression : public Expression {
public:
    BinaryExpression(char op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : operator_(op), left_(std::move(left)), right_(std::move(right)) {}

    double Evaluate(double t, const DynamicTensor& y) const override {
        double left_val = left_->Evaluate(t, y);
        double right_val = right_->Evaluate(t, y);

        switch (operator_) {
            case '+':
                return left_val + right_val;
            case '-':
                return left_val - right_val;
            case '*':
                return left_val * right_val;
            case '/':
                if (right_val == 0) {
                    //MISSING ERROR HANDLING

                }
                return left_val / right_val;
            case '^':
                return std::pow(left_val, right_val);
            default:
                //MISSING ERROR HANDLING
                return 0.0;
        }
    }
private:
    char operator_;
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

/** 
 * @brief Unary Expression Class.
 * Handles unary operations like negation.
 */
class UnaryExpression : public Expression {
public:
    UnaryExpression(char op, std::unique_ptr<Expression> operand)
        : operator_(op), operand_(std::move(operand)) {}

    double Evaluate(double t, const DynamicTensor& y) const override {
        double val = operand_->Evaluate(t, y);

        switch (operator_) {
            case '+':
                return val;
            case '-':
                return -val;
            default:
                //MISSING ERROR HANDLING
                return 0.0;
        }
    }
private:
    char operator_;
    std::unique_ptr<Expression> operand_;
};

/** 
 * @brief Function Expression Class.
 * Handles mathematical functions like sin, cos, exp, log.
 */
class FunctionExpression : public Expression {
public:
    FunctionExpression(const std::string& name, std::unique_ptr<Expression> arg)
        : func_name_(name), argument_(std::move(arg)) {}

    double Evaluate(double t, const DynamicTensor& y) const override {
        double arg_val = argument_->Evaluate(t, y);

        if (func_name_ == "sin") {
            return std::sin(arg_val);
        } else if (func_name_ == "cos") {
            return std::cos(arg_val);
        } else if (func_name_ == "exp") {
            return std::exp(arg_val);
        } else if (func_name_ == "log") {
            if (arg_val <= 0) {
                //MISSING ERROR HANDLING
            }
            return std::log(arg_val);
        } else {
            //MISSING ERROR HANDLING
            return 0.0;
        }
    }
private:
    std::string func_name_;
    std::unique_ptr<Expression> argument_;
};
#endif // EXPRESSION_H

