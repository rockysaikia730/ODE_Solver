//
// Created by andra on 21/11/2025.
//

#ifndef CODES_ODEFUNCTION_H
#define CODES_ODEFUNCTION_H
#include "ODE.h"
#include <vector>
#include <functional>

template <typename  T>
class ODEFunction: public ODE<T> {
    using State = std::vector<T>;
    using Function = std::function<State(double, const State& )>;
public:
    //ODEFunction(const Reader & );

    virtual ~ODEFunction() = default;
    ODEFunction(double t0, const State& y0, const Function& f, const std::string& name);
    ODEFunction(double t0, const T& y0, const Function& f, const std::string& name);
    virtual State evaluate(double t, const State& y) const override;
    virtual State evaluate(double t, const T& y) const override;
private:
    Function rhs;
};
#endif //CODES_ODEFUNCTION_H