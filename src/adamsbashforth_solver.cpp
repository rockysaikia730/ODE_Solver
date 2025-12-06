#include "adamsbashforth_solver.h"
#include "multistep_ode_solver.h"
#include "dynamic_tensor.h"
#include <vector>

AdamsBashforth::AdamsBashforth(const Ode& ode, double step_size, double end_time, int order)
    : MultiStepOdeSolver(ode, step_size, end_time, 1, order),
      order_(order) 
      {
        if(order > 4 || order < 1) throw std::invalid_argument("Order must be within 1 to 4.");
      }

AdamsBashforth::AdamsBashforth(const Ode& ode, int num_of_steps, double end_time, int order)
    : MultiStepOdeSolver(ode, num_of_steps, end_time, 1, order),
      order_(order) {}

const std::vector<double>& AdamsBashforth::GetCoeffsY() const {
    static const std::vector<double> c1 = {1.0, -1.0};
    return c1;
}

const std::vector<double>& AdamsBashforth::GetCoeffsdY() const {
    static const std::vector<double> c1 = {1.0};
    static const std::vector<double> c2 = {1.5, -0.5};
    static const std::vector<double> c3 = {23.0/12.0, -16.0/12.0, 5.0/12.0};
    static const std::vector<double> c4 = {55.0/24.0, -59.0/24.0, 37.0/24.0, -9.0/24.0};
    switch(order_) {
        case 1 : return c1;
        case 2 : return c2;
        case 3 : return c3;
        case 4 : return c4;
        default : return c1;
    }
}

DynamicTensor AdamsBashforth::ComputeStep() {
  return sum_tn_;
}

int AdamsBashforth::GetOrder() const {return order_;}