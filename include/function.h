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
     * @remark When implementing this method, ensure that the output tensor's shape
     *         is compatible with the input tensor's shape as required by the ODE system.
     */
    virtual DynamicTensor Eval(double t, const DynamicTensor& y) const = 0;


    /**
     * @brief Finds Gradient of the function using central difference method.
     * @param t The time variable.
     * @param y The state variable as a DynamicTensor.
     * @return The Gradient of the function evaluation as a DynamicTensor.
     */
    virtual DynamicTensor Grad(double t, const DynamicTensor& y, double dy = 0.1) const;
};
#endif