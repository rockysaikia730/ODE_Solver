#ifndef SOLVER_METHODS_H_
#define SOLVER_METHODS_H_

/**
 * @enum SolverMethod
 * @brief Enumeration of available ODE solver methods.
 * This enum class defines the different numerical methods, so we can easily select which solver to use.
 */
enum class SolverMethod {
    kAdamMoulton,
    kAdamBashforth,
    kBackwardDifferentiation,
    kBackwardEuler,
    kForwardEuler,
    kForwardEulerLight,
    kRungeKutta,
    kUndefinedSolver
};

#endif // SOLVER_METHODS_H_