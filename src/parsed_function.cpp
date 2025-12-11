#include <vector>
#include <complex>
#include <string>
#include <mpParser.h>
#include <iostream>
#include "parsed_function.h"
#include "dynamic_tensor.h"
#include "function.h"

//--------------------------------------------------------------//
// Constructors
//--------------------------------------------------------------//
ParsedFunction::ParsedFunction(const std::vector<std::string>& expressions, const std::vector<size_t>& shape, const std::vector<std::string>& derivatives, const std::vector<size_t>& derivative_shape)
    : expressions_(expressions), function_shape_(shape), derivatives_(derivatives), derivative_shape_(derivative_shape) {
    // Check that the number of expressions matches the shape
    CheckInputShape(expressions_, function_shape_);

    // If derivatives are provided, check their shape as well
    if (!derivatives_.empty()) {
        CheckInputShape(derivatives_, derivative_shape_);
    }
}

//--------------------------------------------------------------//
// Public Methods
//--------------------------------------------------------------//

// Evaluates the parsed function at a given time and state.
DynamicTensor ParsedFunction::Eval(double t, const DynamicTensor& y) const {
    return Compute(expressions_, function_shape_, t, y, true);
}

// Overrides the gradient method if derivative expressions are provided.
DynamicTensor ParsedFunction::Grad(double t, const DynamicTensor& y, double dy) const {
    if (!derivatives_.empty()) {
        return Compute(derivatives_, derivative_shape_, t, y, false); // No shape check for derivatives
    }
    // Fallback to base class implementation if no derivatives are provided
    return Function::Grad(t, y, dy);
}

//--------------------------------------------------------------//
// Private helper methods
//--------------------------------------------------------------//

// Helper method to verify the shape of an input tensor and expted shape. This method could be expanded later to check other dimensions like the derivative tensor.
void ParsedFunction::VerifyDimensions(const std::vector<size_t>& shape, const DynamicTensor& y) const {

    // get shape of input tensor y
    std::vector<size_t> y_shape = y.get_shape();

    //compare if same length of shape and y
    if (y_shape.size() != shape.size()) {
        throw std::invalid_argument("Input tensor rank does not match expected dimensions.");
    }

    //compare to check each dimension
    for (size_t i = 0; i < shape.size(); ++i) {
        if (y_shape[i] != shape[i]) {
            throw std::invalid_argument("Input tensor shape does not match expected dimensions.");
        }
    }
}

// Core computation method to evaluate expressions. LLMs were often consulted on debugging this method. the muparserx documentation can be found here: https://beltoforion.de/en/muparserx/
DynamicTensor ParsedFunction::Compute(const std::vector<std::string>& expressions, const std::vector<size_t>& shape, double t, const DynamicTensor& y, bool check_shape) const {
    
    // Only the function dimension is checked against y. Derivatives have their own shape and we do not check them here.
    if(check_shape){
        VerifyDimensions(shape, y);
    }
    
    // Check if y is complex
    bool is_complex = y.IsComplex();

    size_t N = expressions.size();

    // Prepare the result tensor with appropriate type
    DynamicTensor result = is_complex ? DynamicTensor(shape, DynamicTensor::Complex{0.0, 0.0}) : DynamicTensor(shape, 0.0);

    //Storage for muparserx variables
    mup::Value t_val;
    std::vector<mup::Value> y_val(y.size());

    // defining t variable
    if (is_complex){
        t_val = mup::cmplx_type(t, 0.0);
    }
    else {
        t_val = t;
    }

    // defining y variables
    for (size_t j = 0; j < y.size(); ++j) {
        if (is_complex) {
            auto c = y.flat<DynamicTensor::Complex>(j);
            y_val[j] = mup::cmplx_type(c.real(), c.imag());
        } else {
            y_val[j] = y.flat<double>(j);
        }
    }

    // Evaluate each expression
    for (size_t i = 0; i < N; ++i) {

        mup::ParserX parser;

        // Need to bind variables t and y[i] to each parser instance
        parser.DefineVar("t", mup::Variable(&t_val));

        //define y[i] variables
        for (size_t j = 0; j < y.size(); ++j) {
            parser.DefineVar("y" + std::to_string(j), mup::Variable(&y_val[j]));
        }

        // Set the expression
        parser.SetExpr(expressions[i]);

        // Evaluate the expression
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


// Check the input shape and expressions size match.
void ParsedFunction::CheckInputShape(const std::vector<std::string>& expressions, const std::vector<size_t>& shape) const {
    // Calculate the product of the shape dimensions
    size_t product = 1;
    for (const auto& dim : shape) {
        product *= dim;
    }
    if (expressions.size() != product) {
        throw std::invalid_argument("Number of expressions does not match the specified shape.");
    }
}

            