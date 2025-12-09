#include <vector>
#include <complex>
#include <string>
#include <mpParser.h>
#include <iostream>
#include "parsed_function.h"
#include "dynamic_tensor.h"
#include "function.h"

ParsedFunction::ParsedFunction(const std::vector<std::string>& expressions, const std::vector<size_t>& shape, const std::vector<std::string>& derivatives, const std::vector<size_t>& derivative_shape)
    : expressions_(expressions), function_shape_(shape), derivatives_(derivatives), derivative_shape_(derivative_shape) {
        
        //check if expressions size matches shape
        size_t product = 1;
        for (const auto& dim : function_shape_) {
            product *= dim;
        }
        if (expressions_.size() != product) {
            throw std::invalid_argument("Number of expressions does not match the specified function shape.");
        }

         //check if expressions size matches shape
        if (!derivatives_.empty()) {
            size_t product = 1;
            for (const auto& dim : derivative_shape_) {
                product *= dim;
            }
            if (derivatives_.size() != product) {
                throw std::invalid_argument("Number of derivative expressions does not match the specified derivative shape.");
            }
        }
}


DynamicTensor ParsedFunction::Eval(double t, const DynamicTensor& y) const {
    return Compute(expressions_, function_shape_, t, y, true);
}

bool ParsedFunction::VerifyDimensions(const std::vector<size_t>& shape, const DynamicTensor& y) const {
    std::vector<size_t> y_shape = y.get_shape();

    //compare if same length of shape
    if (y_shape.size() != shape.size()) {
        return false;
    }

    //compare each dimension
    for (size_t i = 0; i < shape.size(); ++i) {
        if (y_shape[i] != shape[i]) {
            return false;
        }
    }
    return true;
}

DynamicTensor ParsedFunction::Compute(const std::vector<std::string>& expressions, const std::vector<size_t>& shape, double t, const DynamicTensor& y, bool check_shape) const {
    
    //verify shape of input y is same as the output
    if(check_shape){
        // Only the function dimension is checked against y. Derivatives have their own shape.
        if (!VerifyDimensions(shape, y)) {
            throw std::invalid_argument("Input tensor shape does not match expected dimensions.");
        }
    }
    
    // Check if y is complex
    bool is_complex = y.IsComplex();
    size_t N = expressions.size();
    DynamicTensor result = is_complex ? DynamicTensor(shape, DynamicTensor::Complex{0.0, 0.0}) : DynamicTensor(shape, double{0.0});
    
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
        parser.SetExpr(expressions[i]);
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

DynamicTensor ParsedFunction::Grad(double t, const DynamicTensor& y, double dy) const {
    if (!derivatives_.empty()) {
        return Compute(derivatives_, derivative_shape_, t, y, false); // No shape check for derivatives
    }
    // Fallback to base class implementation if no derivatives are provided
    return Function::Grad(t, y, dy);
}

            