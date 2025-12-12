#include "multistep_ode_solver.h"
#include "ode_solver.h"
#include "runge_kutta_solver.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

void MultiStepOdeSolver::InitBuffers() {
    solutions_buffer_.clear();
    derivative_buffer_.clear();

    RungeKutta rk4(ode_, step_size_);

    int order = std::max(order_sol_, order_derivative_) - 1;
    if (end_time_ > start_time_) {
        int total_steps = std::ceil((end_time_ - start_time_)/step_size_);
        order = std::min(order, total_steps);
    }

    UpdateSolution(solution_);
    if(order_derivative_) UpdateDerivative(ode_.Evaluate(current_time_, solution_));

    for(int i=0; i < order; i++) {
        rk4.Step();
        solution_ = rk4.GetSolution();
        current_time_ += step_size_;

        UpdateSolution(solution_);
        if(order_derivative_) UpdateDerivative(ode_.Evaluate(current_time_, solution_));
    }
}

void MultiStepOdeSolver::UpdateWindowSum() {
    const auto& coeffs_y = GetCoeffsY();
    const auto& coeffs_dy = GetCoeffsdY();

    std::deque<DynamicTensor>& y_prevs = solutions_buffer_;
    std::deque<DynamicTensor>& dy_prevs = derivative_buffer_;

    auto it_y = y_prevs.rbegin();
    sum_tn_ = ((*it_y) * (coeffs_y[1]/coeffs_y[0]));
    it_y++;
    for(int i = 2; i < coeffs_y.size(); i++) {
        sum_tn_ = (sum_tn_ + (*it_y) * (coeffs_y[i]/coeffs_y[0]));
        it_y++;
    }
    double h = step_size_;
    if(!dy_prevs.empty()) {
        auto it_dy = dy_prevs.rbegin();
        for(int i = 1; i < coeffs_dy.size(); i++) {
            sum_tn_ = ((sum_tn_ +  ((h * (coeffs_dy[i]/coeffs_y[0])) * (*it_dy))));
            it_dy++;
        }    
    }
}

void MultiStepOdeSolver::Reset() {
    OdeSolver::Reset();
    InitBuffers();
    UpdateWindowSum();
}
MultiStepOdeSolver::MultiStepOdeSolver(const Ode& ode, const Reader& reader, 
    int order_solution, int order_derivative)
 : OdeSolver(ode, reader), 
   order_sol_(order_solution),
   order_derivative_(order_derivative) {}

MultiStepOdeSolver::MultiStepOdeSolver(const Ode& ode, double step_size, double end_time, 
    int order_solution, int order_derivative)
    : OdeSolver(ode, step_size, end_time), 
      order_sol_(order_solution),
      order_derivative_(order_derivative)
      {
        if(order_sol_ < 1) throw std::invalid_argument("Order of solution should be >= 1.");
        if(order_derivative_ < 0) throw std::invalid_argument("Order of derivative should be >= 0.");
      }

MultiStepOdeSolver::MultiStepOdeSolver(const Ode& ode, int num_of_steps, double end_time,
    int order_solution, int order_derivative)
    : OdeSolver(ode, num_of_steps, end_time), 
      order_sol_(order_solution),
      order_derivative_(order_derivative)
      {
        if(order_sol_ < 1) throw std::invalid_argument("Order of solution should be >= 1.");
        if(order_derivative_ < 0) throw std::invalid_argument("Order of derivative should be >= 0.");
      }

void MultiStepOdeSolver::UpdateSolution(const DynamicTensor& y) {
    solutions_buffer_.push_back(y);
    if(solutions_buffer_.size() > order_sol_) solutions_buffer_.pop_front();
}

void MultiStepOdeSolver::UpdateDerivative(const DynamicTensor& dydt) {
    derivative_buffer_.push_back(dydt);
    if(derivative_buffer_.size() > order_derivative_) derivative_buffer_.pop_front();
}

void MultiStepOdeSolver::Step() {
    OdeSolver::Step();
    UpdateSolution(solution_);
    if(order_derivative_) UpdateDerivative(ode_.Evaluate(current_time_, solution_));
    UpdateWindowSum();
}