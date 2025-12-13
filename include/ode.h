#ifndef CODES_ODE_H
#define CODES_ODE_H


#include <string>
#include <memory>
#include "dynamic_tensor.h"
#include "reader.h"
#include "function.h"
#include "root_finder.h"
#include "ode_raw_data.h"

/**
 * @class Ode
 * @brief Represents an Ordinary Differential Equation (ODE) problem definition.
 * This class encapsulates the initial conditions, the derivative function,
 * and the root finding strategy for implicit solvers.
 */
class Ode {
public:
    /**
     * @brief Constructor with Tensor state.
     * @param in_time The initial time (t0).
     * @param in_y0 The initial state tensor (y0).
     * @param in_name A unique identifier name for this ODE system.
     * @param func Shared pointer to the function defining dy/dt = f(t, y) and the Jacobian
     * @param root_finder Shared pointer to the root finding strategy.
     */
    Ode(double in_time, const DynamicTensor& in_y0, const std::string& in_name,
        std::shared_ptr<const Function> func, std::shared_ptr<RootFinder> root_finder = nullptr);

    /**
     * @brief Constructor with Scalar state.
     * Convenience constructor that promotes a double to a Rank-1 Tensor.
     * @param in_time The initial time (t0).
     * @param in_y0 The initial state scalar (will be converted to Tensor).
     * @param in_name A unique identifier name for this ODE system.
     * @param func Shared pointer to the function defining dy/dt = f(t, y) and the Jaciobian.
     * @param root_finder Shared pointer to the root finding strategy.
     */
    Ode(double in_time, const double& in_y0, const std::string& in_name,
        std::shared_ptr<const Function> func, std::shared_ptr<RootFinder> root_finder = nullptr);

    /**
     * @brief Constructor from Reader.
     * @param reader A Reader object that provides OdeRawData.
     */
    Ode(const Reader& reader);

    /**
     * @brief Copy Constructor.
     * @param other The Ode object to copy from.
     */
    Ode(const Ode& other);

    /**
     * @brief Copy Assignment Operator.
     * @param other The Ode object to copy from.
     * @return Reference to this Ode object.
     */
    Ode& operator=(const Ode& other);

    /**
     * @brief Constructor from OdeRawData.
     * @param raw An OdeRawData object to initialise the ODE.
     *
     */
    Ode(const OdeRawData& raw);
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~Ode() = default;

    /**
     * @brief Set the initial time.
     * @param time The new initial time as a double.
     */
    void SetT0(double time);

    /**
     * @brief Set the initial state using a Tensor.
     * @param in_y0 The new initial state tensor.
     */
    void SetY0(const DynamicTensor& in_y0);

    /**
     * @brief Set the initial state using a scalar.
     * @param in_y0 The new initial state scalar (promoted to Tensor).
     */
    void SetY0(double in_y0);

    /**
     * @brief Set the name of the ODE system.
     * @param in_name The new name.
     */
    void SetName(const std::string& in_name);

    /**
     * @brief Inject a new derivative function.
     * @param func Shared pointer to the new Function.
     */
    void SetFunction(std::shared_ptr<const Function> func);

    /**
     * @brief Inject a new root finder.
     * @param root_finder Shared pointer to the new RootFinder.
     */
    void SetRootFinder(std::shared_ptr<RootFinder> root_finder);

    /**
     * @brief Get the initial time.
     * @return The initial time (t0).
     */
    double GetTimeIn() const;

    /**
     * @brief Get the initial condition tensor.
     * @return Const reference to the initial state (y0).
     */
    const DynamicTensor& GetCondIn() const;

    /**
     * @brief Get the name of the system.
     * @return The name string.
     */
    const std::string& GetName() const;
    
    /**
     * @brief Get the RHS function.
     * @return Const reference to the Function.
     */
    const Function& GetFunction() const;

    /**
     * @brief Get the RootFinder Routine.
     * @return Const reference to the RootFinder Routine.
     */
    const std::shared_ptr<RootFinder>& GetRootFinder() const;

    /**
     * @brief Evaluate the derivative function f(t, y).
     * * This method delegates the calculation to the stored Function object.
     * * @param t The current time.
     * @param y The current state tensor.
     * @return A DynamicTensor representing dy/dt.
     */
    DynamicTensor Evaluate(double t, const DynamicTensor& y) const;

    /**
     * @brief Evaluate the gradient of the derivative function.
     * @param t The current time.
     * @param y The current state tensor.
     * @return A DynamicTensor representing the gradient.
     * @remark This method uses the Function's Grad method.
     */
    DynamicTensor Grad(double t, const DynamicTensor& y) const;

private:
    /// @brief Initial time.
    double t0_;

    /// @brief Initial state tensor.
    DynamicTensor y0_;

    /// @brief Identifier name.
    std::string name_;

    /// @brief Pointer to the derivative function logic.
    std::shared_ptr<const Function> func_;

    /// @brief Pointer to the root finding logic (optional/strategy pattern).
    std::shared_ptr<RootFinder> root_finder_;
};

#endif // CODES_ODE_H
