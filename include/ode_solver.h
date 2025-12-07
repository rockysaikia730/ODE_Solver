/**
 * @file ode_solver.h
 * @brief Definition of the abstract base class for ODE Solvers.
 *
 * This file defines the generic interface for solving Ordinary Differential
 * Equations. Concrete implementations (e.g., ForwardEuler, RungeKutta4)
 * should inherit from this class and implement the Step() method.
 */

#ifndef CODES_ODE_SOLVER_H
#define CODES_ODE_SOLVER_H


#include <cmath>
#include "ode.h"
#include "dynamic_tensor.h"

/**
 * @class OdeSolver
 * @brief Abstract base class for iterative Ordinary Differential Equation solvers.
 *
 * The OdeSolver manages the simulation loop, time tracking, and state updates.
 * It holds a reference to the problem definition (Ode) and uses a specific
 * numerical method (implemented in derived classes via Step()) to advance the solution.
 */
class OdeSolver {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~OdeSolver() = default;

    /**
     * @brief Calculates the solution at the next time step.
     *
     * This is a pure virtual function that must be implemented by derived classes.
     * It uses the specific numerical algorithm (e.g., RK4, Euler) to calculate
     * y(t + dt) based on y(t).
     *
     * @return The calculated state tensor for the next time step.
     */
    virtual DynamicTensor ComputeStep() = 0;

    /**
     * @brief Updates the solution_ and time for next time step.
     */
    virtual void Step();

    /**
     * @brief Sets the simulation end time.
     * @param end_time The target time t_end.
     */
    void SetEndTime(double end_time);

    /**
     * @brief Configures the solver based on a fixed number of steps.
     *
     * Automatically calculates and updates the step_size_ based on the formula:
     * dt = (end_time - start_time) / num_of_steps.
     *
     * @param num_of_steps The total number of iterations to perform.
     */
    void SetTimeSpan(int num_of_steps);

    /**
     * @brief Configures the solver based on a fixed step size.
     * @param step_size The time delta (dt) for each iteration.
     */
    void SetTimeSpan(double step_size);

    // --- Getters ---

    /**
     * @return The final solution.
     */
    const DynamicTensor& GetSolution() const;

    /**
     * @return The configured start time.
     */
    double GetStartTime() const;

    /**
     * @return The configured end time.
     */
    double GetEndTime() const;

    /**
     * @return The current time step size (dt).
     */
    double GetStepSize() const;

    /**
     * @return The derived number of steps based on duration and step size.
     */
    int GetNumberOfSteps() const;

    /**
     * @return The current time
     */
    double GetCurrentTime() const;

    

    // --- Control Flow ---

    /**
     * @brief Resets the solver state to the initial conditions.
     *
     * Sets current_time_ to start_time_ and current solution_ to ode_.GetCondIn().
     * This must be called before restarting a simulation.
     */
    virtual void Reset();

    /**
     * @brief Executes the simulation loop (Batch Mode).
     *
     * Runs the simulation from current_time_ until end_time_ is reached.
     * Internally calls Step() repeatedly and updates the internal state.
     */
    void Solve();

    // --- Constructors ---

    /**
     * @brief Constructor for fixed number of steps.
     *
     * @param ode A constant reference to the ODE definition. Must outlive the solver.
     * @param end_time The target simulation time.
     * @param num_of_steps The number of intervals to divide the time span into.
     */
    OdeSolver(const Ode& ode, int num_of_steps, double end_time);

    /**
     * @brief Constructor for fixed step size.
     *
     * @param ode A constant reference to the ODE definition. Must outlive the solver.
     * @param end_time The target simulation time.
     * @param step_size The time step (dt). Defaults to 0.01.
     */
    OdeSolver(const Ode& ode, double step_size = 0.01, double end_time = 0.0);

protected:
    /**
     * @brief Reference to the ODE problem definition.
     * Guaranteed to be valid as long as the user maintains the Ode object scope.
     */
    const Ode& ode_;

    /// @brief The simulation start time.
    double start_time_;

    /// @brief The simulation target end time.
    double end_time_;

    /// @brief The time step (dt).
    double step_size_;

    /// @brief The current state y(t). Updated at every Step().
    DynamicTensor solution_;

    /// @brief The current simulation time t. Updated at every Step().
    double current_time_;
};
#endif // CODES_ODE_SOLVER_H