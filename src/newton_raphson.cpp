#include "dynamic_tensor.h"
#include "newton_raphson.h"

DynamicTensor NewtonRaphson::FindRoot(const Function& f, const DynamicTensor& initial_guess, double t, int max_iter) const {
    DynamicTensor sol = initial_guess;
    for(int i=0; i<max_iter; i++) {
        sol = (sol - (f.Eval(t, sol) / f.Grad(t, sol)));
    }
    return sol;
}