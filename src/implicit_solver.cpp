#include "implicit_solver.h"
#include "newton_raphson.h"
#include "dynamic_tensor.h"
#include <vector>
#include <deque>

ImplicitSolver::ImplicitEquation::ImplicitEquation(const ImplicitSolver& solver)
    : solver_(solver) {
        //c0 yn + sum_{i=1} cy_i y_n-i = cdy_0 f(yn) + sum_{i} cdy_i f(y_n-i) 
        cy0_  = solver_.GetCoeffsY()[0];
        cdy0_ = solver_.GetCoeffsdY()[0];
    }

DynamicTensor ImplicitSolver::ImplicitEquation::Eval(double t, const DynamicTensor& y_guess) const {
    double h = solver_.step_size_;
    DynamicTensor dydt = solver_.ode_.Evaluate(t + h, y_guess);
    return y_guess + solver_.sum_tn_ - (dydt * (h * cdy0_/cy0_));
}

ImplicitSolver::ImplicitSolver(const Ode& ode, double step_size, double end_time, 
    int order_solution, int order_derivative, std::shared_ptr<RootFinder> root_finder)
    : MultiStepOdeSolver(ode, step_size, end_time, 
        order_solution, order_derivative) 
      {
        if(root_finder) root_finder_ = std::move(root_finder);
        root_finder_ = ode_.GetRootFinder();
        if(!root_finder_) root_finder_ = std::make_shared<NewtonRaphson>();
      }

ImplicitSolver::ImplicitSolver(const Ode& ode, int num_of_steps, double end_time,
    int order_solution, int order_derivative, std::shared_ptr<RootFinder> root_finder)
    : MultiStepOdeSolver(ode, 
                        num_of_steps, 
                        end_time, 
                        order_solution, 
                        order_derivative) 
      {
        if(root_finder) root_finder_ = std::move(root_finder);
        root_finder_ = ode_.GetRootFinder();
        if(!root_finder_) root_finder_ = std::make_shared<NewtonRaphson>();
      }

DynamicTensor ImplicitSolver::ComputeStep() {
    double t = current_time_ + step_size_;
    double h = step_size_;
    ImplicitEquation implicit_equation(*this, t, h);
    //What is a good initial guess
    return root_finder_->FindRoot(implicit_equation, GetSolution());
}