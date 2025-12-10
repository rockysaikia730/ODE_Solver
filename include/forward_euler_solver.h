#ifndef FORWARD_EULER_H_
#define FORWARD_EULER_H_

#include "adamsbashforth_solver.h"

class ForwardEuler : public AdamsBashforth {
public:
    ForwardEuler(const Ode& ode, double step_size = 0.01, double end_time = 0.0);
    ForwardEuler(const Ode& ode, int num_of_steps, double end_time = 0.0);
};
#endif