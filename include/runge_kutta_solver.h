#ifndef RUNGE_KUTTA_SOLVER_H_
#define RUNGE_KUTTA_SOLVER_H_

#include "ode.h"
#include "ode_solver.h"

class RungeKutta : public OdeSolver {
public:
    RungeKutta(const Ode& ode, double step_size = 0.01, double end_time = 0.0);
    RungeKutta(const Ode& ode, int num_of_steps, double end_time);

    DynamicTensor ComputeStep() override;
};

#endif