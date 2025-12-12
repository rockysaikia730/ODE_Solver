#include "runge_kutta_solver.h"
#include "ode_solver.h"

RungeKutta::RungeKutta(const Ode& ode, const Reader& reader) 
    : OdeSolver(ode, reader) {}


RungeKutta::RungeKutta(const Ode& ode, double step_size, double end_time) 
    : OdeSolver(ode, step_size, end_time) {}

RungeKutta::RungeKutta(const Ode& ode, int num_of_steps, double end_time) 
    : OdeSolver(ode, num_of_steps, end_time) {}

DynamicTensor RungeKutta::ComputeStep() {
    static const double coeffs[3][2] = {
                                        {0.5, 0.5},
                                        {0.5, 0.5},
                                        {1.0, 1.0}
                                        };
    static const double sum_kcoeffs[4] = {1.0/6.0, 1.0/3.0, 1.0/3.0, 1.0/6.0};

    const DynamicTensor& y = GetSolution();
    double t = current_time_;
    double h = step_size_;

    DynamicTensor k = ode_.Evaluate(t, y) * h;
    DynamicTensor sum_k = k * sum_kcoeffs[0];

    for(int i=0; i < 3; i++) {
        double t_next = t + coeffs[i][0]*h;
        DynamicTensor y_next = y + (coeffs[i][1] * k);
        k = ode_.Evaluate(t_next, y_next) * h;
        sum_k = sum_k + (k * sum_kcoeffs[i+1]);
    }
    return y + sum_k;
}