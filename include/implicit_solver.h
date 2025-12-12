#ifndef IMPLICIT_SOLVER_H_
#define IMPLICIT_SOLVER_H_

#include "multistep_ode_solver.h"
#include "root_finder.h"
#include <memory>
#include <vector>

/**
 * @class ImplicitSolver
 * @brief Multistep ODE solver that performs implicit time-stepping using a root-finding algorithm.
 *
 * This class implements an implicit multistep solver for ordinary differential equations (ODEs).
 * At each time step, a nonlinear system arising from the implicit scheme is solved using a
 * provided RootFinder instance.
 */
class ImplicitSolver : public MultiStepOdeSolver {
public:
    ImplicitSolver(const Ode& ode, const Reader& reader, int order_solution, 
        int order_derivative, std::shared_ptr<RootFinder> root_finder);
    /**
     * @brief Constructs an ImplicitSolver using a fixed step size.
     *
     * @param ode               Reference to the ODE system to integrate.
     * @param step_size         Time step size.
     * @param end_time          Final integration time.
     * @param order_solution    Order of the multistep scheme for the solution.
     * @param order_derivative  Order of the multistep scheme for the derivative.
     * @param root_finder       Shared pointer to a RootFinder used to solve implicit equations.
     */
    ImplicitSolver(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
                   int order_solution = 1, int order_derivative = 0, 
                   std::shared_ptr<RootFinder> root_finder = nullptr);
    /**
     * @brief Constructs an ImplicitSolver using a specified number of time steps.
     *
     * @param ode               Reference to the ODE system to integrate.
     * @param num_of_steps      Number of time steps to divide the interval into.
     * @param end_time          Final integration time.
     * @param order_solution    Order of the multistep solution formula.
     * @param order_derivative  Order of the multistep derivative formula.
     * @param root_finder       Shared pointer to a RootFinder used to solve implicit equations.
     */
    ImplicitSolver(const Ode& ode, int num_of_steps, double end_time = 0.0, 
                   int order_solution = 1, int order_derivative = 0,
                   std::shared_ptr<RootFinder> root_finder = nullptr);
    /**
     * @brief Advances the solution by one implicit time step.
     *
     * This method sets up the implicit equation for the next time step and
     * uses the assigned RootFinder to solve for the next state.
     *
     * @return The next state vector after the time step.
     */
    virtual DynamicTensor ComputeStep() override;
protected:   
    /**
     * @class ImplicitEquation
     * @brief Nonlinear equation representing the implicit update rule at a single time step.
     *
     * The implicit multistep method produces an equation of the form:
     * \f[
     *     F(t_{n+1},y_{n+1}) = 0
     * \f]
     * This class provides the evaluation and finite-difference gradient needed by the RootFinder.
     */
    class ImplicitEquation : public Function {
    public:
        ImplicitEquation(const ImplicitSolver& solver);
        /**
         * @brief Evaluates the nonlinear implicit equation at a given guess for y.
         *
         * @param t         Current time.
         * @param y_guess   Current guess for the next-step solution.
         * @return          Value of the implicit equation.
         */
        DynamicTensor Eval(double t, const DynamicTensor& y_guess) const override;
        /**
         * @brief Computes the gradient of the implicit equation w.r.t. y.
         *
         * Uses finite differences with an optional increment size.
         *
         * @param t         Current time.
         * @param y_guess   Current guess for the solution.
         * @param dy        Finite-difference step size.
         * @return          Approximate gradient tensor.
         */
        DynamicTensor Grad(double t, const DynamicTensor& y_guess, double dy = 0.1) const override;
    private: 
        /**
         * @brief Reference to the parent implicit solver.
         */
        const ImplicitSolver& solver_;
        /**
         * @brief Cached coefficient associated with the derivative term
         *        in the implicit multistep formula.
         */
        double cdy0_;
        /**
         * @brief Cached coefficient associated with the solution term
         *        in the implicit multistep formula.
         */
        double cy0_;
    };
    /**
     * @brief Root finder used to solve the nonlinear implicit equation at each step.
     */
    std::shared_ptr<RootFinder> root_finder_;
};
#endif