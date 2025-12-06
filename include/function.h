#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <dynamic_tensor.h>
#include <memory>

/**
 * @class Function
 * @brief Abstract base class representing a mathematical function for ODEs.
 * This class defines the interface for evaluating functions that take a time
 * and a DynamicTensor as input and return a DynamicTensor as output.
 */
class Function {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~Function() = default;
    
    /**
     * @brief Evaluates the function at a given time and state.
     * @param t The time variable.
     * @param y The state variable as a DynamicTensor.
     * @return The result of the function evaluation as a DynamicTensor.
     */
    virtual DynamicTensor Eval(double t, const DynamicTensor& y) const = 0;

    /**
     * @brief Creates a deep copy of the Function object.
     * This is useful to be able to copy an ODE problem including its function.
     * @return A unique pointer to the cloned Function object.
     */
    virtual std::unique_ptr<Function> Clone() const = 0;
};
#endif