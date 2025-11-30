#ifndef CODES_ODE_H
#define CODES_ODE_H

#include <vector>
#include <string>
#include <dynamic_tensor.h>
#include "function.h"
#include "root_finder.h"

class Ode {
public:
    Ode(double inTime, const DynamicTensor& inY0, const std::string& in_name);
    Ode(double inTime, const double& inY0, const std::string& in_name);

    virtual ~Ode();

    void SetT0(const double in_time);
    void SetY0(const DynamicTensor& in_y0);
    void SetY0(double& in_y0);
    void SetName(const std::string& in_name);

    double GetTimeIn() const{ return t0_;};
    const DynamicTensor& GetCondIn() const;
    const std::string& GetName() const;

    DynamicTensor Evaluate(double t, const DynamicTensor& y) const;
private:
    double t0_;
    DynamicTensor y0_;
    std::string name_;
    const Function& func_;
    const RootFinder& root_finder_;
};
#endif