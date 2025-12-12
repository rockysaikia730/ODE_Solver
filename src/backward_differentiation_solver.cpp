#include "backward_differentiation_solver.h"
#include "implicit_solver.h"
#include "ode.h"
#include "dynamic_tensor.h"
#include "root_finder.h"
#include <vector>


Bdf::Bdf(const Ode& ode, double step_size, double end_time, int order, 
         std::shared_ptr<RootFinder> root_finder)
        : ImplicitSolver(ode, step_size, end_time, order, 0, root_finder),
          order_(order) 
          {
            if(order_ < 1 || order_ > 4) throw std::invalid_argument("Order must be within 1 to 4.");
            Reset();
          }

Bdf::Bdf(const Ode& ode, int num_of_steps, double end_time, int order, 
        std::shared_ptr<RootFinder> root_finder)
        : ImplicitSolver(ode, num_of_steps, end_time, order, 0, root_finder),
          order_(order) 
          {
            if(order_ < 1 || order_ > 4) throw std::invalid_argument("Order must be within 1 to 4.");
            Reset();
          }

const std::vector<double>& Bdf::GetCoeffsY() const {
    static const std::vector<double> c1 = {1.0, 1.0};
    static const std::vector<double> c2 = {1.0, 4.0/3.0, -1.0/3.0};
    static const std::vector<double> c3 = {1.0, 18.0/11.0, -9.0/11.0, 2.0/11.0};
    static const std::vector<double> c4 = {1.0, 48.0/25.0, -36.0/25.0, 16.0/25.0, -3.0/25.0};

    switch(order_) {
        case 1 : return c1;
        case 2 : return c2;
        case 3 : return c3;
        case 4 : return c4;
        default : return c1;
    }
}

const std::vector<double>& Bdf::GetCoeffsdY() const {
    static const std::vector<double> c1 = {1.0};
    static const std::vector<double> c2 = {2.0/3.0};
    static const std::vector<double> c3 = {6.0/11.0};
    static const std::vector<double> c4 = {12.0/25.0};

    switch(order_) {
        case 1 : return c1;
        case 2 : return c2;
        case 3 : return c3;
        case 4 : return c4;
        default : return c1;
    }
}

int Bdf::GetOrder() {return order_;}
