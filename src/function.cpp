#include "function.h"
#include "dynamic_tensor.h"
#include <memory>


// Numerical gradient using central difference. Only for real-valued, scalar functions.
DynamicTensor Function::Grad(double t, const DynamicTensor& y, double dy) const {
    return (Eval(t , y + dy) - Eval(t, y - dy)) / (2*dy);
}
