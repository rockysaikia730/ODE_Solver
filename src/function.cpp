#include "function.h"
#include "dynamic_tensor.h"
#include <memory>
#include <stdexcept>


// Numerical gradient using central difference. Only for real-valued, scalar functions.
DynamicTensor Function::Grad(double t, const DynamicTensor& y, double dy) const {
    if(dy <= 0) throw std::runtime_error("Divison by zero for function gradient.");
    return (Eval(t , y + dy) - Eval(t, y - dy)) / (2*dy);
}
