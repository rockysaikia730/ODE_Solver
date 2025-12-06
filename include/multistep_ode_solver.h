#ifndef MULTISTEP_ODE_SOLVER_H_
#define MULTISTEP_ODE_SOLVER_H_

#include "ode_solver.h"
#include "dynamic_tensor.h"
#include <deque>

class MultiStepOdeSolver : public OdeSolver {
public:
    MultiStepOdeSolver(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
                        int order_solution = 1, int order_derivative = 1);
    MultiStepOdeSolver(const Ode& ode, int num_of_steps, double end_time = 0.0,
                       int order_solution = 1, int order_derivative = 1);

    virtual void InitBuffers();
    void UpdateDerivative(const DynamicTensor& dydt);
    void UpdateSolution(const DynamicTensor& y);
    void UpdateWindowSum();

    void Reset() override;
    void Step() override;

    virtual const std::vector<double>& GetCoeffsY() const = 0;
    virtual const std::vector<double>& GetCoeffsdY() const = 0;
protected:
    int order_sol_;
    int order_derivative_;
    DynamicTensor sum_tn_;
    std::deque<DynamicTensor> solutions_buffer_;
    std::deque<DynamicTensor> derivative_buffer_;
};
#endif