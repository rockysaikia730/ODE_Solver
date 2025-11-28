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
    void setT0(const double inTime){t0_ = inTime;};
    void setY0(const State& inY0){y0_ = inY0;};//Verify copy
    void setY0(const T& inY0) {y0_ = {inY0};};
    void setName(const std::string& inName){name_ = inName;};
    double getTimeIn() const{ return t0_;};
    const State& getCondIn() const{ return y0_;};
    const std::string& getName() const{ return name_;};
    virtual State evaluate(double t, const State& y) const = 0;
    virtual State evaluate(double t, const T& y) const = 0;

    ODE(double inTime, const State& inY0, const std::string& inName) {
        t0_ = inTime;
        y0_ = inY0;
        name_ = inName;
    }
    ODE(double inTime, const T& inY0, const std::string& inName) {
        t0_ = inTime;
        y0_ = {inY0};
        name_ = inName;
    }
private:
    double t0_;
    State y0_;
    std::string name_;

};
#endif //CODES_ODE_H