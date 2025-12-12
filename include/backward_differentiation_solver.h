#ifndef BACKWARD_DIFFERENTIATION_H_
#define BACKWARD_DIFFERENTIATION_H_

#include "ode.h"
#include <vector>
#include "dynamic_tensor.h"
#include "implicit_solver.h"
#include "reader.h"

/**
 * @class Bdf
 * @brief Implicit multistep ODE solver implementing the Backward Differentiation Formula (BDF).
 *
 * The BDF family consists of implicit multistep methods used primarily for stiff ODE systems.
 * At each step, a nonlinear equation must be solved, which is handled through the
 * ImplicitSolver base class and a provided RootFinder implementation.
 */
class Bdf : public ImplicitSolver {
public:
    /**
     * @brief Constructs a BDF solver using a fixed step size.
     *
     * @param ode            Reference to the ODE system being integrated.
     * @param step_size      Time step size.
     * @param end_time       Final integration time.
     * @param order          Order of the BDF method (commonly up to 6; typical default is 4).
     * @param root_finder    Shared pointer to a RootFinder used to solve the implicit equation.
     */
    Bdf(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
        int order = 4, std::shared_ptr<RootFinder> root_finder = nullptr);
    /**
     * @brief Constructs a BDF solver using a specified number of time steps.
     *
     * @param ode             Reference to the ODE system being integrated.
     * @param num_of_steps    Number of uniform time steps over the integration interval.
     * @param end_time        Final integration time.
     * @param order           Order of the BDF method.
     * @param root_finder     Shared pointer to a RootFinder used to solve the implicit equation.
     */
    Bdf(const Ode& ode, int num_of_steps, double end_time = 0.0, 
        int order = 4, std::shared_ptr<RootFinder> root_finder = nullptr);

    /**
     * @brief Returns the order of the BDF method used by the solver.
     *
     * @return BDF order as an integer.
     */
    int GetOrder();
    /**
     * @brief Returns the coefficients applied to solution values in the BDF formula.
     *
     * These coefficients correspond to the linear multistep representation:
     * \f[
     *     \sum_{k=0}^{m} \alpha_k y_{n-k} = h \beta f(t_{n+1}, y_{n+1})
     * \f]
     *
     * @return Constant reference to the vector of BDF solution coefficients.
     */
    const std::vector<double>& GetCoeffsY() const override;
    /**
     * @brief Returns the derivative coefficient(s) used in the BDF scheme.
     *
     * For all BDF methods, the right-hand side uses a single derivative coefficient \f$\beta\f$.
     *
     * @return Constant reference to the vector of derivative coefficients.
     */
    const std::vector<double>& GetCoeffsdY() const override;
private:
    /**
     * @brief Order of the Backward Differentiation Formula being used.
     */
    int order_;
};

#endif