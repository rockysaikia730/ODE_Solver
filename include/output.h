#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "ode_solver.h"
/**
 * @brief Abstract base class for outputting ODE solver results.
 * Derived classes should implement the Write method to handle specific output formats.
 */
class Output {
public:
    /**
     * @brief Default constructor and destructor.
     */
    Output() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Output() = default;

    /**
     * @brief Method to write the ODE solver results.
     * @param solver A constant reference to the OdeSolver instance containing results to output.
     */
    virtual void Write(const OdeSolver& solver) = 0;
};
#endif // OUTPUT_H_