#include "adam_moulton_solver.h"
#include "implicit_solver.h"
#include <stdexcept>

AdamMoulton::AdamMoulton(const Ode& ode, double step_size, double end_time, 
                int order, std::shared_ptr<RootFinder> root_finder)
        : ImplicitSolver(ode, step_size, end_time, 1, order, root_finder),
          order_(order) {
            if(order > 4 || order < 1) throw std::invalid_argument("Order must be within 1 to 4.");
            Reset();
          }

AdamMoulton::AdamMoulton(const Ode& ode, int num_of_steps, double end_time, 
                int order, std::shared_ptr<RootFinder> root_finder)
        : ImplicitSolver(ode, num_of_steps, end_time, 1, order, root_finder),
          order_(order) {
            if(order > 4 || order < 1) throw std::invalid_argument("Order must be within 1 to 4.");
            Reset();
          }

const std::vector<double>& AdamMoulton::GetCoeffsY() const {
    static const std::vector<double> c1 = {1.0, 1.0};
    return c1;
}

const std::vector<double>& AdamMoulton::GetCoeffsdY() const {
    static const std::vector<double> c1 = {1.0};
    static const std::vector<double> c2 = {1.0/2.0, 1.0/2.0};
    static const std::vector<double> c3 = {5.0/12.0, 8.0/12.0, -1.0/12.0};
    static const std::vector<double> c4 = {9.0/24.0, 19.0/24.0, -5.0/24.0, 1.0/24.0};

    switch(order_) {
        case 1 : return c1;
        case 2 : return c2;
        case 3 : return c3;
        case 4 : return c4;
        default : return c1;
    }
}

int AdamMoulton::GetOrder() {return order_;}
