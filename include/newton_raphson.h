#ifndef NEWTON_RAPHSON_H_
#define NEWTON_RAPHSON_H_

#include "root_finder.h"

class NewtonRaphson : public RootFinder {
public:
    DynamicTensor FindRoot(const Function& f, const DynamicTensor& initial_guess, double t, int max_iter = 500) const override;
};
#endif