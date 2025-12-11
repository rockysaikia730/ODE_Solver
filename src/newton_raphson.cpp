#include "dynamic_tensor.h"
#include "newton_raphson.h"

DynamicTensor NewtonRaphson::FindRoot(const Function& f, const DynamicTensor& initial_guess, double t, int max_iter) const {
    DynamicTensor sol = initial_guess;
    for(int i=0; i<max_iter; i++) {
        DynamicTensor gradient = f.Grad(t, sol);
        if(gradient.IsComplex()) {
            auto it_end = gradient.end<DynamicTensor::Complex>();
            for(auto it = gradient.begin<DynamicTensor::Complex>(); it != it_end; it++) {
                if(*it == DynamicTensor::Complex(0.0, 0.0)) { 
                    throw std::runtime_error("Division by 0 in Newton Raphson.");
                }
            }
        }
        else {
           auto it_end = gradient.end<double>();
            for(auto it = gradient.begin<double>(); it != it_end; it++) {
                if(*it == 0.0) { 
                    throw std::runtime_error("Division by 0 in Newton Raphson.");
                }
            } 
        }
        sol = (sol - (f.Eval(t, sol) / gradient));
    }
    return sol;
}