#ifndef PARSED_FUNCTION_H_
#define PARSED_FUNCTION_H_
#include <vector>
#include <string>
#include <memory>
#include <muParserX.h>
#include "function.h"
#include "dynamic_tensor.h"

class ParsedFunction : public Function {
public:
    /**
     * @brief Constructor that initializes the ParsedFunction with given expressions.
     * @param expressions A vector of strings representing the mathematical expressions for each output dimension.
     */
    ParsedFunction(const std::vector<std::string>& expressions);

    /**
     * @brief Copy Constructor.
     * @param other The ParsedFunction object to copy from.
     */
    ParsedFunction(const ParsedFunction& other);

    /**
     * @brief Copy Assignment Operator.
     * @param other The ParsedFunction object to assign from.
     * @return A reference to the assigned ParsedFunction object.
     */
    ParsedFunction& operator=(const ParsedFunction& other);

    /**
     * @brief Creates a deep copy of the ParsedFunction object.
     * @return A unique pointer to the cloned ParsedFunction object.
     */
    std::unique_ptr<Function> Clone() const override;

    /**
     * @brief Evaluates the function at a given time and state.
     * @param t The time variable.
     * @param y The state variable as a DynamicTensor.
     * @return The result of the function evaluation as a DynamicTensor.
     */
    DynamicTensor Eval(double t, const DynamicTensor& y) const override;

private:
    mutable mup::ParserX parser_;
    mutable double t_;
    mutable std::vector<mup::Value> y_vars_;
    std::vector<size_t> state_shape_;

    void AssignVariables();
};
#endif // PARSED_FUNCTION_H_