#include <vector>
#include <complex>
#include <string>
#include <mpParser.h>
#include <iostream>
#include "parsed_function.h"
#include "dynamic_tensor.h"
#include "function.h"

ParsedFunction::ParsedFunction(const std::vector<std::string>& expressions, const std::vector<size_t>& shape)
    : expressions_(expressions), shape_(shape) {
}

std::unique_ptr<Function> ParsedFunction::Clone() const {
    return std::make_unique<ParsedFunction>(*this);
}


DynamicTensor ParsedFunction::Eval(double t, const DynamicTensor& y) const {
    bool is_complex = y.IsComplex();

    size_t N = expressions_.size();
    DynamicTensor result = is_complex ? DynamicTensor(shape_, DynamicTensor::Complex{0.0, 0.0}) : DynamicTensor(shape_, double{0.0});
    //persistent storage for parser objects
    mup::Value t_val;
    std::vector<mup::Value> y_val(y.size());

    for (size_t i = 0; i < N; ++i) {
        mup::ParserX parser;

        // defining t variable
        if (is_complex){
            t_val = mup::cmplx_type(t, 0.0);
        }
        else {
            t_val = t;
        }

        parser.DefineVar("t", mup::Variable(&t_val));

        //define y[i] variables
        for (size_t j = 0; j < y.size(); ++j) {
            if (is_complex) {
                auto c = y.flat<DynamicTensor::Complex>(j);
                y_val[j] = mup::cmplx_type(c.real(), c.imag());
            } else {
                y_val[j] = y.flat<double>(j);
            }
            parser.DefineVar("y" + std::to_string(j), mup::Variable(&y_val[j]));
        }

        // Set the expression
        parser.SetExpr(expressions_[i]);
        mup::Value val = parser.Eval();
        // Assign the result in the output tensor
        if (!is_complex) {
            result.flat<double>(i) = val.GetFloat();
        }
        else {
            auto c = val.GetComplex();
            result.flat<DynamicTensor::Complex>(i) = DynamicTensor::Complex(c.real(), c.imag());
        }
    }

    return result;
}

const std::vector<size_t>& ParsedFunction::GetShape() const {
    return shape_;
}