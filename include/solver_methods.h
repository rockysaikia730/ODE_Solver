#ifndef SOLVER_METHODS_H
#define SOLVER_METHODS_H

/**
 * @enum SolverMethod
 * @brief Enumeration of available ODE solver methods.
 */
enum class SolverMethod {
    kExplicitEuler,
    kImplicitEuler,
    kRungeKutta4,
    kAdamsBashforth,
    kUnknown
};

#endif // SOLVER_METHODS_H