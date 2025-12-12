#ifndef BACKWARD_EULER_SOLVER_H_
#define BACKWARD_EULER_SOLVER_H_

#include "ode.h"
#include "backward_differentiation_solver.h"

/**
 * @class BackwardEuler
 * @brief Implicit first-order ODE solver using the Backward Euler method.
 *
 * The Backward Euler method is a special case of the Backward Differentiation Formula (BDF)
 * with order 1. It is an implicit method suitable for stiff ODEs, requiring a RootFinder
 * to solve the nonlinear equation at each time step.
 */
class BackwardEuler: public Bdf {
public:

    /**
     * @brief Constructs a Backward Euler solver using input from a Reader.
     * @param ode         Reference to the ODE system to integrate.
     * @param reader      Reader object containing solver configuration.
     * @param root_finder Shared pointer to a RootFinder for implicit solves.
     */
    BackwardEuler(const Ode& ode, const Reader& reader, std::shared_ptr<RootFinder> root_finder = nullptr); 
    /**
     * @brief Constructs a Backward Euler solver using a fixed step size.
     *
     * @param ode            Reference to the ODE system to integrate.
     * @param step_size      Time step size.
     * @param end_time       Final integration time.
     * @param root_finder    Shared pointer to a RootFinder for implicit solves.
     */
    BackwardEuler(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
                  std::shared_ptr<RootFinder> root_finder = nullptr);
    /**
     * @brief Constructs a Backward Euler solver using a specified number of steps.
     *
     * @param ode             Reference to the ODE system to integrate.
     * @param num_of_steps    Number of uniform time steps.
     * @param end_time        Final integration time.
     * @param root_finder     Shared pointer to a RootFinder for implicit solves.
     */
    BackwardEuler(const Ode& ode, int num_of_steps, double end_time = 0.0, 
                  std::shared_ptr<RootFinder> root_finder = nullptr);
};
#endif