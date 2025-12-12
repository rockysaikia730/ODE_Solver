#include "forward_euler_solver.h"
#include "adamsbashforth_solver.h"

ForwardEuler::ForwardEuler(const Ode& ode, const Reader& reader) 
    : AdamsBashforth(ode, reader){}

ForwardEuler::ForwardEuler(const Ode& ode, double step_size, double end_time)
    : AdamsBashforth(ode, step_size, end_time, 1) {}

ForwardEuler::ForwardEuler(const Ode& ode, int num_of_steps, double end_time)
    : AdamsBashforth(ode, num_of_steps, end_time, 1)  {}
