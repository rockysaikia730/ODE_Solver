#include "backward_euler_solver.h"
#include "function.h"
#include "ode.h"
#include "backward_differentiation_solver.h"


BackwardEuler::BackwardEuler(const Ode& ode, double step_size, double end_time, 
      std::shared_ptr<RootFinder> root_finder)
    : Bdf(ode, step_size, end_time, 1, root_finder) {}

BackwardEuler::BackwardEuler(const Ode& ode, int num_of_steps, double end_time, 
                  std::shared_ptr<RootFinder> root_finder)
    : Bdf(ode, num_of_steps, end_time, 1, root_finder)  {}