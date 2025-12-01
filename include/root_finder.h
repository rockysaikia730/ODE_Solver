#ifndef ROOT_FINDER_H_
#define ROOT_FINDER_H_

#include <dynamic_tensor.h>
#include "function.h"

class RootFinder {
public:
    virtual ~RootFinder() = default;
    virtual DynamicTensor FindRoot(const Function& f, const DynamicTensor& initial_guess) const = 0;
};
#endif