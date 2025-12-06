#include <vector>
#include <complex>
#include <string>
#include <muParserX.h>
#include "parsed_function.h"
#include "dynamic_tensor.h"
#include "function.h"

using Value::mup::Value;

ParsedFunction::ParsedFunction(const std::vector<std::string>& expressions)
    : t_(0.0), output_dim_(expressions.size()) {
    
        //Allocate number of equations
        parser_.SetNumOutputs(static_cast<int>(output_dim_));

        //Prepare y variables
        AssignVariables();

        //set expressions
        for (size_t i = 0; i < output_dim_; ++i) {
            parser_.SetExpr(static_cast<int>(i), expressions[i]);
        }
}

ParsedFunction::ParsedFunction(const ParsedFunction& other)
    : parser_(other.parser_), t_(other.t_), y_vars_(other.y_vars_), output_dim_(other.output_dim_) {
    AssignVariables();
}

ParsedFunction& ParsedFunction::operator=(const ParsedFunction& other) {
    if (this != &other) {
        parser_ = other.parser_;
        t_ = other.t_;
        y_vars_ = other.y_vars_;
        output_dim_ = other.output_dim_;
        AssignVariables();
    }
    return *this;
}

std::unique_ptr<Function> ParsedFunction::Clone() const {
    return std::make_unique<ParsedFunction>(*this);
}

void ParsedFunction::AssignVariables() {
    parser_.DefineVar("t", &t_);

    if (!y_vars_.empty()) {
        for (size_t i = 0; i < y_vars_.size(); ++i) {
            std::string name = "y" + std::to_string(i);
            parser_.DefineVar(name, &y_vars_[i]);
        }
    }

}

DynamicTensor ParsedFunction::Eval(double t, const DynamicTensor& y) const {
    t_ = t;

    state_shape_ = y.get_shape();
    const size_t n = y.size();

    //ensuring size match
    if (y.size() != y_vars_.size()) {
        y_vars_.assign(y.size(), 0.0);
        AssignVariables();
    }

    if (!y.IsComplex()) {
        for(size_t i = 0; i < n; ++i) {
            y_vars_[i] = Value(y.flat<double>()[i]);
    }
    else {
        for(size_t i = 0; i < n; ++i) {
            y_vars_[i] = Value(y.flat<DynamicTensor::Complex>()[i]);
        }
    }

    std::vector<Value> results = parser_.Eval();
    bool is_any_complex = false;

    for (const auto& res : results) {
        if (res.GetType() == 'c' || res.GetType() == 'm'){
            is_any_complex = true;
            break;
        }
    }

    if (!is_any_complex) {
        DynamicTensor result(state_shape_, 0.0);
        for (size_t i = 0; i < n; ++i) {
            result.flat<double>()[i] = results[i].GetFloat();
        }
        return result;
    }
    else{
        //At least one complex result
        DynamicTensor result(state_shape_, DynamicTensor::Complex(0.0, 0.0));
        for (size_t i = 0; i < n; ++i) {
            result.flat<DynamicTensor::Complex>()[i] = results[i].GetComplex();
        }
        return result;
    }


}
