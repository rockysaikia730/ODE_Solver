//
// Created by andra on 21/11/2025.
//

#ifndef CODES_ODEFUNCTION_H
#define CODES_ODEFUNCTION_H
#include "ODE.h"
#include "Reader.h"
#include <vector>
#include <functional>

template <typename  T>
class ODEFunction: public ODE<T> {
    using State = std::vector<T>;
    using Function = std::function<State(double, const State& )>;
public:
    //ODEFunction(const Reader & );
    ODEFunction(double t0, double y0, std::function<State(double t, const State& y)>);
    virtual State evaluate(double t, const State& y) const;
private:
    Function rhs;
};
#endif //CODES_ODEFUNCTION_H