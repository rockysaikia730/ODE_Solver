#ifndef FORWARD_EULER_LIGHT_SOLVER_H_
#define FORWARD_EULER_LIGHT_SOLVER_H_

#include "dynamic_tensor.h"
#include "ode_solver.h"

class ForwardEulerLight: public OdeSolver {
public:
    ForwardEulerLight(const Ode& ode, double step_size = 0.01, double end_time = 0.0);
    ForwardEulerLight(const Ode& ode, int num_of_steps, double end_time = 0.0);

    DynamicTensor ComputeStep() override;
};
#endif