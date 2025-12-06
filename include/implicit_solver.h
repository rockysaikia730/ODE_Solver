#ifndef IMPLICIT_SOLVER_H_
#define IMPLICIT_SOLVER_H_

#include "multistep_ode_solver.h"
#include "root_finder.h"
#include <memory>
#include <vector>

class ImplicitSolver : public MultiStepOdeSolver {
public:
    ImplicitSolver(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
                   int order_solution = 1, int order_derivative = 0, 
                   std::shared_ptr<RootFinder> root_finder = nullptr);
    ImplicitSolver(const Ode& ode, int num_of_steps, double end_time = 0.0, 
                   int order_solution = 1, int order_derivative = 0,
                   std::shared_ptr<RootFinder> root_finder = nullptr);
    
    virtual const std::vector<double>& GetCoeffsY() const = 0;
    virtual const std::vector<double>& GetCoeffsdY() const = 0;

    virtual DynamicTensor ComputeStep() override;
protected:   
    class ImplicitEquation : public Function {
    public:
        ImplicitEquation(const ImplicitSolver& solver);
        DynamicTensor Eval(double t, const DynamicTensor& y_guess) const override;
    private: 
        const ImplicitSolver& solver_;
        double cdy0_;
        double cy0_;
    };

    std::shared_ptr<RootFinder> root_finder_;
};
#endif