#ifndef ADAMSBASHFORTH_SOLVER_H_
#define ADAMSBASHFORTH_SOLVER_H_

#include "ode.h"
#include "multistep_ode_solver.h"
#include "dynamic_tensor.h"
#include <vector>

class AdamsBashforth : public MultiStepOdeSolver {
public:
    AdamsBashforth(const Ode& ode, double step_size = 0.01, double end_time = 0.0,  int order = 4);
    AdamsBashforth(const Ode& ode, int num_of_steps, double end_time = 0.0, int order = 4);

    DynamicTensor ComputeStep() override;

    const std::vector<double>& GetCoeffsY() const override;
    const std::vector<double>& GetCoeffsdY() const override;
    
    int GetOrder() const;
private:
    int order_;
};
#endif