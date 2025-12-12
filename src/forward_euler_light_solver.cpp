#include "dynamic_tensor.h"
#include "ode_solver.h"
#include "ode.h"
#include "forward_euler_light_solver.h"


ForwardEulerLight::ForwardEulerLight(const Ode& ode, double step_size, double end_time)
    : OdeSolver(ode, step_size, end_time) {}

ForwardEulerLight::ForwardEulerLight(const Ode& ode, int num_of_steps, double end_time)
        : OdeSolver(ode, num_of_steps, end_time) {}

DynamicTensor ForwardEulerLight::ComputeStep() {
        const DynamicTensor& y = solution_;
        DynamicTensor dydt = ode_.Evaluate(current_time_, y);
        return y + (dydt * step_size_);
}
