#ifndef ADAM_MOULTON_SOLVER_H_
#define ADAM_MOULTON_SOLVER_H_

#include "implicit_solver.h"
#include "ode.h"
#include "reader.h"

/**
 * @class AdamMoulton
 * @brief Implicit multistep ODE solver implementing the Adams–Moulton method.
 *
 * The Adams–Moulton family consists of implicit linear multistep methods.
 * Unlike Adams–Bashforth, these methods require solving a nonlinear system
 * at each step but generally offer higher stability and accuracy for stiff
 * or moderately stiff ODEs. This class uses the ImplicitSolver interface
 * along with a RootFinder to perform each implicit solve.
 */
class AdamMoulton : public ImplicitSolver {
public: 
    /**
     * @brief Constructs an Adams–Moulton solver using input from a Reader.
     * @param ode            Reference to the ODE system to integrate.
     * @param reader         Reader object containing solver configuration.
     * @param root_finder    Shared pointer to a RootFinder used to solve the implicit equation.
     */
    AdamMoulton(const Ode& ode, const Reader& reader, std::shared_ptr<RootFinder> root_finder = nullptr);
    /**
     * @brief Constructs an Adams–Moulton solver using a fixed step size.
     *
     * @param ode            Reference to the ODE system to integrate.
     * @param step_size      Time step size.
     * @param end_time       Final integration time.
     * @param order          Order of the Adams–Moulton method (commonly 1–4).
     * @param root_finder    Shared pointer to a RootFinder used to solve the implicit equation.
     */
    AdamMoulton(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
                int order = 2, std::shared_ptr<RootFinder> root_finder = nullptr);
    /**
     * @brief Constructs an Adams–Moulton solver using a specified number of time steps.
     *
     * @param ode             Reference to the ODE system to integrate.
     * @param num_of_steps    Number of uniform time steps.
     * @param end_time        Final integration time.
     * @param order           Order of the Adams–Moulton method.
     * @param root_finder     Shared pointer to a RootFinder for implicit solves.
     */
    AdamMoulton(const Ode& ode, int num_of_steps, double end_time = 0.0, 
                int order = 2, std::shared_ptr<RootFinder> root_finder = nullptr);
    /**
     * @brief Returns the coefficients applied to solution values in the implicit Adams–Moulton formula.
     *
     * These coefficients appear in the linear multistep representation:
     * \f[
     *     \sum_{k=0}^{m} \alpha_k y_{n+1-k}
     *      = h \sum_{k=0}^{m} \beta_k f(t_{n+1-k}, y_{n+1-k})
     * \f]
     *
     * @return Constant reference to the vector of solution coefficients.
     */
    const std::vector<double>& GetCoeffsY() const override;
    /**
     * @brief Returns the derivative coefficients used in the Adams–Moulton formula.
     *
     * These coefficients determine the weighting of previous derivative evaluations
     * on the right-hand side of the implicit step equation.
     *
     * @return Constant reference to the vector of derivative coefficients.
     */
    const std::vector<double>& GetCoeffsdY() const override;
    /**
     * @brief Returns the order of the Adams–Moulton scheme.
     *
     * @return Integer order of the method.
     */
    int GetOrder();
private: 
    /**
     * @brief Order of the Adams–Moulton method (number of steps used).
     */
    int order_;

};
#endif