#ifndef ROOT_FINDER_H_
#define ROOT_FINDER_H_

#include <dynamic_tensor.h>
#include "function.h"

/**
 * @class RootFinder
 * @brief Abstract base class for root-finding algorithms.
 *
 * This interface defines the contract for classes that implement methods to
 * find roots of nonlinear functions. Derived classes must implement the
 * FindRoot method.
 */
class RootFinder {
public:
    /**
     * @brief Virtual destructor for safe polymorphic use.
     */
    virtual ~RootFinder() = default;
    /**
     * @brief Finds the root of a nonlinear function starting from an initial guess.
     *
     * Derived classes implement a specific root-finding algorithm (e.g., Newton–Raphson).
     *
     * @param f              Function whose root is to be found.
     * @param initial_guess  Initial guess for the root.
     * @param t              Current time (used if the function is time-dependent).
     * @param max_iter       Maximum number of iterations (default 100).
     * @return               Estimated root as a DynamicTensor.
     */
    virtual DynamicTensor FindRoot(const Function& f, const DynamicTensor& initial_guess, double t, int max_iter = 100) const = 0;
};
#endif