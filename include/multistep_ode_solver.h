#ifndef MULTISTEP_ODE_SOLVER_H_
#define MULTISTEP_ODE_SOLVER_H_

#include "ode_solver.h"
#include "dynamic_tensor.h"
#include <deque>


/**
 * @class MultiStepOdeSolver
 * @brief Base class for multistep ODE solvers using previous solution
 *        and derivative values.
 */
class MultiStepOdeSolver : public OdeSolver {
public:
    /**
     * @brief Constructor with fixed step size.
     * @param ode            ODE system to solve.
     * @param step_size      Time step size.
     * @param end_time       End time of integration.
     * @param order_solution Order of the multistep method for y.
     * @param order_derivative Order of the multistep method for dy/dt.
     */
    MultiStepOdeSolver(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
                        int order_solution = 1, int order_derivative = 1);

    /**
     * @brief Constructor using a Reader.
     * @param reader         Reader object to configure the solver.
     * @param ode            ODE system to solve.
     * @param order_solution Order of the multistep method for y.
     */
    MultiStepOdeSolver(const Reader& reader, const Ode& ode, 
                       int order_solution = 1, int order_derivative = 1);
    
    /**
     * @brief Constructor using a specified number of steps.
     * @param ode            ODE system to solve.
     * @param num_of_steps   Number of steps to take.
     * @param end_time       End time of integration.
     * @param order_solution Order of the multistep method for y.
     * @param order_derivative Order of the multistep method for dy/dt.
     */
    MultiStepOdeSolver(const Ode& ode, int num_of_steps, double end_time = 0.0,
                       int order_solution = 1, int order_derivative = 1);

    /**
     * @brief Advance the solution by one time step.
     */
    void Step() override;
    /**
     * @brief Get the coefficients for combining past solution values.
     * @return Vector of solution coefficients.
     */
    virtual const std::vector<double>& GetCoeffsY() const = 0;
    /**
     * @brief Get the coefficients for combining past derivative values.
     * @return Vector of derivative coefficients.
     */
    virtual const std::vector<double>& GetCoeffsdY() const = 0;

    /**
     * @brief Reset solver state and history buffers.
     */
    void Reset() override;

protected:
    /**
     * @brief Initialize buffers for storing solution and derivative history.
     */
    virtual void InitBuffers();

    /**
     * @brief Add a new derivative value to the history.
     * @param dydt New derivative value.
     */
    void UpdateDerivative(const DynamicTensor& dydt);

    /**
     * @brief Add a new solution value to the history.
     * @param y New solution value.
     */
    void UpdateSolution(const DynamicTensor& y);

    /**
     * @brief Update cached weighted sums used by the multistep scheme.
     */
    void UpdateWindowSum();

    /** @brief Order of the multistep term using past solution values. */
    int order_sol_;

    /** @brief Order of the multistep term using past derivative values. */
    int order_derivative_;

    /** @brief Buffer of previously computed solution states. */
    std::deque<DynamicTensor> solutions_buffer_;

    /** @brief Buffer of previously computed derivative evaluations. */
    std::deque<DynamicTensor> derivative_buffer_;

    /** @brief Cached weighted sum of stored solution/derivative terms. */
    DynamicTensor sum_tn_;
};
#endif