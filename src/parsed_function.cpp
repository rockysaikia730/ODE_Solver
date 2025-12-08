#include <vector>
#include <complex>
#include <string>
#include <mpParser.h>
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
    DynamicTensor result(shape_, is_complex ? DynamicTensor::Complex(0.0, 0.0) : 0.0);

    for (size_t i = 0; i < N; ++i) {
        mup::ParserX parser;

        //define t
        mup::Value t_var = t;
        mup::Variable t_var_obj(&t_var);
        parser.DefineVar("t", t_var_obj);

        //define y[i] variables
        for (size_t j = 0; j < y.size(); ++j) {
            if (is_complex) {
                auto c = y.flat<DynamicTensor::Complex>(j);
                mup::Value v(mup::cmplx_type(c.real(), c.imag()));
                mup::Variable var(&v);
                parser.DefineVar("y" + std::to_string(j), var);
            }
            else {
                mup::Value v(y.flat<double>(j));
                mup::Variable var(&v);
                parser.DefineVar("y" + std::to_string(j), var);
            }
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