#include <iostream>
#include <cmath>
#include <memory>

#include "output_plotter.h" 
#include "adamsbashforth_solver.h"
#include "ode.h"
#include "function.h"
#include "dynamic_tensor.h"



class SimpleSineODE : public Function {
public:
    DynamicTensor Eval(double t, const DynamicTensor& y) const override {
        return DynamicTensor(cos(t));
    }
};

int main() {
    try {
        std::cout << "--- Starting Adams-Bashforth Visual Test ---" << std::endl;
        auto rhs_ptr = std::make_shared<SimpleSineODE>();
        
        double t0 = 0.0;
        
        Ode exponential_ode(t0, 0.0, "Exponential Decay", rhs_ptr);
        AdamsBashforth ab4(exponential_ode); 

        OutputPlotter plotter;

        int iter = 500;
        for(int i = 0; i < iter; i++) {
            ab4.Step();
            plotter.Write(ab4);
        }

        double current_time = ab4.GetCurrentTime();
        double current_y = ab4.GetSolution().at<double>({0});
        double exact_y = std::exp(-current_time);

        std::cout << "\n--- Done ---" << std::endl;
        std::cout << "Time: " << current_time << std::endl;
        std::cout << "Computed y: " << current_y << std::endl;
        std::cout << "Exact y:    " << exact_y << std::endl;
        std::cout << "Error:      " << std::abs(current_y - exact_y) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Test Failed with Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}