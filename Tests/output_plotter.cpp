#include <iostream>
#include <cmath>
#include <memory>

#include "output_plotter.h" 
#include "adamsbashforth_solver.h"
#include "ode.h"
#include "function.h"
#include "dynamic_tensor.h"
#include "gtest/gtest.h"

class SinOde : public Function {
public:
    DynamicTensor Eval(double t, const DynamicTensor& y) const override {
        return DynamicTensor(cos(t));
    }
};

TEST(OutputPlotter, Plotting) {
    auto rhs_ptr = std::make_shared<SinOde>();
        
    double t0 = 0.0;

    Ode exponential_ode(t0, 0.0, "Exponential Decay", rhs_ptr);
    AdamsBashforth ab4(exponential_ode); 

    OutputPlotter plotter;

    int iter = 500;
    for(int i = 0; i < iter; i++) {
        ab4.Step();
        plotter.Write(ab4);
    }
}
