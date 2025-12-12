#ifndef NEWTON_RAPHSON_H_
#define NEWTON_RAPHSON_H_

#include "root_finder.h"

/**
 * @class NewtonRaphson
 * @brief Root-finding algorithm implementing the Newton–Raphson method.
 *
 * This class provides an implementation of the Newton–Raphson method to find
 * roots of a nonlinear system defined by a Function. It iteratively updates the
 * guess using the derivative (Jacobian) until convergence or a maximum number of iterations.
 */
class NewtonRaphson : public RootFinder {
public:
    /**
     * @brief Solves for the root of a nonlinear function using Newton–Raphson iterations.
     *
     * The method updates the initial guess according to:
     * \f[
     *     y_{k+1} = y_k - J^{-1}(y_k) F(y_k)
     * \f]
     * where \f$J\f$ is the Jacobian of \f$F\f$ at \f$y_k\f$.
     *
     * @param f             Function whose root is to be found.
     * @param initial_guess Initial guess for the root.
     * @param t             Current time (used if the function is time-dependent).
     * @param max_iter      Maximum number of iterations (default 250).
     * @return              Estimated root as a DynamicTensor.
     */
    DynamicTensor FindRoot(const Function& f, const DynamicTensor& initial_guess, double t, int max_iter = 250) const override;
};
#endif