#ifndef FORWARD_EULER_LIGHT_SOLVER_H_
#define FORWARD_EULER_LIGHT_SOLVER_H_

#include "dynamic_tensor.h"
#include "ode_solver.h"

/**
 * @class ForwardEulerLight
 * @brief Simple explicit first-order ODE solver using the Forward Euler method.
 *
 * This class implements the Forward Euler method, a basic explicit solver for
 * ordinary differential equations. It is suitable for non-stiff problems and
 * advances the solution by a single fixed step at a time.
 */
class ForwardEulerLight: public OdeSolver {
public:
    ForwardEulerLight(const Ode& ode, const Reader& reader);
    /**
     * @brief Constructs a Forward Euler solver using a fixed step size.
     *
     * @param ode         Reference to the ODE system to integrate.
     * @param step_size   Time step size.
     * @param end_time    Final integration time.
     */
    ForwardEulerLight(const Ode& ode, double step_size = 0.01, double end_time = 0.0);
     /**
     * @brief Constructs a Forward Euler solver using a specified number of steps.
     *
     * @param ode           Reference to the ODE system to integrate.
     * @param num_of_steps  Number of uniform time steps.
     * @param end_time      Final integration time.
     */
    ForwardEulerLight(const Ode& ode, int num_of_steps, double end_time = 0.0);

    /**
     * @brief Advances the solution by one Forward Euler time step.
     *
     * Uses the formula:
     * \f[
     *     y_{n+1} = y_n + h f(t_n, y_n)
     * \f]
     *
     * @return State vector at the next time step.
     */
    DynamicTensor ComputeStep() override;
};
#endif