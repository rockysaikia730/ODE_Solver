#ifndef RUNGE_KUTTA_SOLVER_H_
#define RUNGE_KUTTA_SOLVER_H_

#include "ode.h"
#include "ode_solver.h"

/**
 * @class RungeKutta
 * @brief Explicit Runge–Kutta ODE solver.
 *
 * This class implements a classical 4th-order Runge–Kutta method
 * for solving ordinary differential equations. It is an explicit solver suitable
 * for non-stiff problems, providing higher accuracy than Forward Euler.
 */
class RungeKutta : public OdeSolver {
public:
    /**
     * @brief Constructs a Runge–Kutta solver using a fixed step size.
     *
     * @param ode         Reference to the ODE system to integrate.
     * @param step_size   Time step size.
     * @param end_time    Final integration time.
     */
    RungeKutta(const Ode& ode, double step_size = 0.01, double end_time = 0.0);
     /**
     * @brief Constructs a Runge–Kutta solver using a specified number of steps.
     *
     * @param ode           Reference to the ODE system to integrate.
     * @param num_of_steps  Number of uniform time steps.
     * @param end_time      Final integration time.
     */
    RungeKutta(const Ode& ode, int num_of_steps, double end_time);

    /**
     * @brief Advances the solution by one Runge–Kutta time step.
     *
     * Typically implements the classical 4th-order Runge–Kutta formula:
     * \f[
     *     y_{n+1} = y_n + \frac{h}{6}(k_1 + 2 k_2 + 2 k_3 + k_4)
     * \f]
     * where \f$k_i\f$ are slope estimates at intermediate points.
     *
     * @return State vector at the next time step.
     */
    DynamicTensor ComputeStep() override;
};

#endif