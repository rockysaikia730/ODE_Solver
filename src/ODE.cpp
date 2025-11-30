#include "ODE.h"

#include <vector>
#include <string>

Ode::Ode(double in_time, const DynamicTensor& in_y0, const std::string& in_name)
    : t0_(in_time),
      y0_(in_y0),
      name_(in_name) {}

Ode::Ode(double in_time, const double& in_y0, const std::string& in_name)
    : t0_(in_time),
      y0_({in_y0}),
      name_(in_name) {}

Ode::~Ode() {}

DynamicTensor Ode::Evaluate(double t, const DynamicTensor& y) const {
    return func_.eval(t, y)
}

const std::string& Ode::GetName() const {
    return name_;
};

const DynamicTensor& Ode::GetCondIn() const {
    return y0_;
}
double Ode::GetTimeIn() const {
    return t0_;
}

void Ode::SetT0(double time) {
  t0_ = time;
}

void Ode::SetY0(const DynamicTensor& y0) {
  y0_ = y0;
}

void Ode::SetY0(double y0) {
  y0_ = DynamicTensor({y0});
}

void Ode::SetName(const std::string& name) {
  name_ = name;
}