#include "function.h"
#include "dynamic_tensor.h"
#include <memory>


DynamicTensor Function::Grad(double t, const DynamicTensor& y, double dy) const {
    return (Eval(t , y + dy) - Eval(t, y - dy)) / (2*dy);
}
