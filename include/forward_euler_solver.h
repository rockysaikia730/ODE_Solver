#ifndef FORWARD_EULER_H_
#define FORWARD_EULER_H_

#include "adamsbashforth_solver.h"

/**
 * @class ForwardEuler
 * @brief Explicit first-order ODE solver using the Forward Euler method.
 *
 * The Forward Euler method is a special case of the Adams–Bashforth method with order 1.
 * It is suitable for non-stiff ODEs and advances the solution explicitly by a single step:
 * \f[
 *     y_{n+1} = y_n + h f(t_n, y_n)
 * \f]
 */
class ForwardEuler : public AdamsBashforth {
public:

    /**
     * @brief Constructs a Forward Euler solver using input from a Reader.
     * @param ode         Reference to the ODE system to integrate.
     * @param reader      Reader object containing solver configuration.
     */
    ForwardEuler(const Ode& ode, const Reader& reader);
    /**
     * @brief Constructs a Forward Euler solver using a fixed step size.
     *
     * @param ode         Reference to the ODE system to integrate.
     * @param step_size   Time step size.
     * @param end_time    Final integration time.
     */    
    ForwardEuler(const Ode& ode, double step_size = 0.01, double end_time = 0.0);

    /**
     * @brief Constructs a Forward Euler solver using a specified number of steps.
     *
     * @param ode           Reference to the ODE system to integrate.
     * @param num_of_steps  Number of uniform time steps.
     * @param end_time      Final integration time.
     */
    ForwardEuler(const Ode& ode, int num_of_steps, double end_time = 0.0);
};
#endif