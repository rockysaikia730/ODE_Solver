#ifndef ADAMSBASHFORTH_SOLVER_H_
#define ADAMSBASHFORTH_SOLVER_H_

#include "ode.h"
#include "multistep_ode_solver.h"
#include "dynamic_tensor.h"
#include <vector>

/**
 * @class AdamsBashforth
 * @brief Explicit multistep ODE solver implementing the Adams–Bashforth method.
 *
 * This class provides an explicit multistep method for solving ordinary differential equations.
 * The Adams–Bashforth scheme uses previous derivative evaluations to compute the next state
 * without requiring implicit root-solving, making it efficient for non-stiff systems.
 */
class AdamsBashforth : public MultiStepOdeSolver {
public:
    AdamsBashforth(const Ode& ode, const Reader& reader);
    /**
     * @brief Constructs an Adams–Bashforth solver using a fixed step size.
     *
     * @param ode         Reference to the ODE system to integrate.
     * @param step_size   Integration time step.
     * @param end_time    Final integration time.
     * @param order       Order of the Adams–Bashforth method (typically 1–4).
     */
    AdamsBashforth(const Ode& ode, double step_size = 0.01, double end_time = 0.0,  int order = 4);

    /**
     * @brief Constructs an Adams–Bashforth solver using a specified number of time steps.
     *
     * @param ode           Reference to the ODE system to integrate.
     * @param num_of_steps  Number of uniform time steps used in the integration interval.
     * @param end_time      Final integration time.
     * @param order         Order of the Adams–Bashforth method.
     */
    AdamsBashforth(const Ode& ode, int num_of_steps, double end_time = 0.0, int order = 4);

    /**
     * @brief Advances the solution by one Adams–Bashforth time step.
     *
     * Uses the explicit Adams–Bashforth formula:
     * \f[
     *     y_{n+1} = y_n + h \sum_{k=0}^{p-1} b_k f(t_{n-k}, y_{n-k})
     * \f]
     *
     * @return State vector at the next time step.
     */
    DynamicTensor ComputeStep() override;
    /**
     * @brief Returns the coefficients applied to solution values in the multistep formula.
     *
     * For Adams–Bashforth, this typically returns zeros because the method uses only
     * derivative terms. Included for compatibility with the MultiStepOdeSolver interface.
     *
     * @return Constant reference to the vector of coefficients.
     */
    const std::vector<double>& GetCoeffsY() const override;
    /**
     * @brief Returns the derivative coefficients for the Adams–Bashforth formula.
     *
     * These coefficients correspond to the explicit multistep weights associated with
     * previous derivative evaluations.
     *
     * @return Constant reference to the vector of derivative coefficients.
     */
    const std::vector<double>& GetCoeffsdY() const override;
    /**
     * @brief Returns the order of the Adams–Bashforth scheme used by the solver.
     *
     * @return Order of the method.
     */
    int GetOrder() const;
private:
    /**
     * @brief Order of the Adams–Bashforth integration method.
     */
    int order_;
};
#endif