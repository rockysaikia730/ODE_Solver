#ifndef BACKWARD_EULER_SOLVER_H_
#define BACKWARD_EULER_SOLVER_H_

#include "ode.h"
#include "backward_differentiation_solver.h"

class BackwardEuler: public Bdf {
public:
    BackwardEuler(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
                  std::shared_ptr<RootFinder> root_finder = nullptr);
    BackwardEuler(const Ode& ode, int num_of_steps, double end_time = 0.0, 
                  std::shared_ptr<RootFinder> root_finder = nullptr);
};
#endif