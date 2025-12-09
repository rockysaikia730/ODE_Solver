#ifndef PARSED_FUNCTION_H_
#define PARSED_FUNCTION_H_

#include <vector>
#include <string>
#include <memory>
#include <mpParser.h>
#include "function.h"
#include "dynamic_tensor.h"

/**
 * @class ParsedFunction
 * @brief Concrete implementation of Function using muParserX for parsing mathematical expressions.
 *
 * This class allows users to define ODE functions using string expressions that are parsed
 * and evaluated at runtime. It supports multiple output expressions and can handle tensor inputs.
 */
class ParsedFunction : public Function {
public:
    /**
     * @brief Constructor.
     * @param expressions A vector of string expressions representing the function outputs.
     * @param shape The shape of the output tensor.
     * @param derivatives (Optional) A vector of string expressions representing the derivatives.
     * @param derivative_shape (Optional) The shape of the derivative output tensor.
     */
    ParsedFunction(const std::vector<std::string>& expressions, const std::vector<size_t>& shape, const std::vector<std::string>& derivatives = {}, const std::vector<size_t>& derivative_shape = {});
    
    /**
     * @brief Evaluates the parsed function at a given time and state.
     * @param t The time variable.
     * @param y The state variable as a DynamicTensor.
     * @return The result of the function evaluation as a DynamicTensor.
     */
    DynamicTensor Eval(double t, const DynamicTensor& y) const override;

    /**
     * @brief Overrides the gradient method if derivative expressions are provided.
     * @param t The time variable.
     * @param y The state variable as a DynamicTensor.
     * @return The Gradient of the function evaluation as a DynamicTensor.
     */
    DynamicTensor Grad(double t, const DynamicTensor& y, double dy = 0.1) const override;

private:
    /**
     * @brief Expressions to be parsed and evaluated.
     */
    std::vector<std::string> expressions_;

    /**
     * @brief Shape of the output tensor.
     */
    std::vector<size_t> function_shape_;

    /**
     * @brief Derivative expressions (optional).
     * @remark Overwrites the gradient method if provided.
     */
    std::vector<std::string> derivatives_;
    /**
     * @brief Shape of the derivative output tensor.
     */
    std::vector<size_t> derivative_shape_;

    /**
     * @brief Helper method to verify input tensor shape.
     * @param y The input state tensor.
     * @return True if dimensions are compatible, false otherwise.
     */
    bool VerifyDimensions(const std::vector<size_t>& shape, const DynamicTensor& y) const;


    /**
     * @brief Core computation method to evaluate expressions.
     * @param expressions The vector of string expressions to evaluate.
     * @param shape The shape of the output tensor.
     * @param t The time variable.
     * @param y The state variable as a DynamicTensor.
     * @return The result of the computation as a DynamicTensor.
     * @remark This method is used by both Eval and Grad, depending on the context.
     */
    DynamicTensor Compute(const std::vector<std::string>& expressions, const std::vector<size_t>& shape, double t, const DynamicTensor& y, bool check_shape) const;
};
#endif // PARSED_FUNCTION_H_