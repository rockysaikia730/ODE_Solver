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
     */
    ParsedFunction(const std::vector<std::string>& expressions, const std::vector<size_t>& shape);

    /**
     * @brief Create a deep copy of the ParsedFunction object.
     * @return A unique pointer to the cloned ParsedFunction object.
     */
    std::unique_ptr<Function> Clone() const override;
    
    /**
     * @brief Evaluates the parsed function at a given time and state.
     * @param t The time variable.
     * @param y The state variable as a DynamicTensor.
     * @return The result of the function evaluation as a DynamicTensor.
     */
    DynamicTensor Eval(double t, const DynamicTensor& y) const override;

private:

    /**
     * @brief Expressions to be parsed and evaluated.
     */
    std::vector<std::string> expressions_;

    /**
     * @brief Shape of the output tensor.
     */
    std::vector<size_t> shape_;
};
#endif // PARSED_FUNCTION_H_