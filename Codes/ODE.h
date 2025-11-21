//
// Created by andra on 21/11/2025.
//

#ifndef CODES_ODE_H
#define CODES_ODE_H

#include <vector>
template <typename T>
class ODE {
    using State = std::vector<T>;
private:
    double t0;
    State y0;
public:
    virtual ~ODE();
    void setT0(double inTime){t0 = inTime;};
    void setY0(double inY0){y0 = inY0;};//Verify copy
    double getTimeIn() const{ return t0;};
    State getCondIn() const{ return y0;};
    virtual State evaluate(double t, const State &y) const = 0;
};
#endif //CODES_ODE_H