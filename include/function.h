#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <dynamic_tensor.h>

class Function {
public:
    Function() {};
    virtual DynamicTensor Eval(double t, const DynamicTensor& y) const = 0;
};
#endif