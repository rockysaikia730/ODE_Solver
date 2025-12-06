#include "function.h"
#include "dynamic_tensor.h"

DynamicTensor Function::Grad(double t, const DynamicTensor& y, double h) {
    return (Eval(t + h , y) - Eval(t - h, y)) / (2*h)
}