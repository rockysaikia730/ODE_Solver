#ifndef BACKWARD_DIFFERENTIATION_H_
#define BACKWARD_DIFFERENTIATION_H_

#include "ode.h"
#include <vector>
#include "dynamic_tensor.h"
#include "implicit_solver.h"

class Bdf : public ImplicitSolver {
public:
    Bdf(const Ode& ode, double step_size = 0.01, double end_time = 0.0, 
        int order = 4, std::shared_ptr<RootFinder> root_finder = nullptr);
    Bdf(const Ode& ode, int num_of_steps, double end_time = 0.0, 
        int order = 4, std::shared_ptr<RootFinder> root_finder = nullptr);

    int GetOrder();
    const std::vector<double>& GetCoeffsY() const override;
    const std::vector<double>& GetCoeffsdY() const override;
private:
    int order_;
};

#endif