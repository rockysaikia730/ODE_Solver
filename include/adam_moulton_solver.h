#ifndef ADAM_MOULTON_SOLVER _H_
#define ADAM_MOULTON_SOLVER

#include "implicit_solver.h"
#include "ode.h"

class AdamMoulton : public ImplicitSolver {
public: 
    AdamMoulton(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
                int order = 1, std::shared_ptr<RootFinder> root_finder = nullptr);
    AdamMoulton(const Ode& ode, int num_of_steps, double end_time = 0.0, 
                int order = 1, std::shared_ptr<RootFinder> root_finder = nullptr);
    
    const std::vector<double>& GetCoeffsY() const override;
    const std::vector<double>& GetCoeffsdY() const override;

    int GetOrder();
public: 
    int order_;

};
#endif