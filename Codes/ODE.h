//
// Created by andra on 21/11/2025.
//

#ifndef CODES_ODE_H
#define CODES_ODE_H

#include <vector>
#include <string>
template <typename T>
class ODE {
public:
    using State = std::vector<T>;
    virtual ~ODE() = default;
    void setT0(double inTime){t0 = inTime;};
    void setY0(double inY0){y0 = inY0;};//Verify copy
    void setName(const std::string& inName){name = inName;};
    double getTimeIn() const{ return t0;};
    const State& getCondIn() const{ return y0;};
    const std::string& getName() const{ return name;};
    virtual State evaluate(double t, const State& y) const = 0;
    virtual State evaluate(double t, const T& y) const = 0;

    ODE(double inTime, const State& inY0, const std::string& inName) {
        t0 = inTime;
        y0 = inY0;
        name = inName;
    }
    ODE(double inTime, const T& inY0, const std::string& inName) {
        t0 = inTime;
        y0 = {inY0};
        name = inName;
    }
private:
    double t0;
    State y0;
    std::string name;

};
#endif //CODES_ODE_H