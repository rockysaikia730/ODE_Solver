#ifndef ROOT_FINDER_H_
#define ROOT_FINDER_H_

#include <dynamic_tensor.h>

class RootFinder {
public:
    RootFinder() {};
    virtual DynamicTensor FindRoot(double t, DynamicTensor y) = 0;
};
#endif