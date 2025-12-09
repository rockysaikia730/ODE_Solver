#include "ode.h"
#include "dynamic_tensor.h"
#include "function.h"
#include "root_finder.h"
#include "ode_raw_data.h"
#include "reader.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>


Ode::Ode(double in_time, const DynamicTensor& in_y0, const std::string& in_name,
         std::shared_ptr<const Function> func, std::shared_ptr<const Function> derivative, std::shared_ptr<RootFinder> root_finder)
    : t0_(in_time),
      y0_(in_y0),
      name_(in_name),
      func_(func),
      root_finder_(root_finder) {
        func_->Eval(t0_, y0_); // verify dimensions at construction
    }

Ode::Ode(double in_time, const double& in_y0, const std::string& in_name,
         std::shared_ptr<const Function> func, std::shared_ptr<const Function> derivative, std::shared_ptr<RootFinder> root_finder)
    : t0_(in_time),
      y0_(in_y0),
      name_(in_name),
      func_(func),
      root_finder_(root_finder) {
        func_->Eval(t0_, y0_); // verify dimensions at construction
      }

Ode::~Ode() {}

Ode::Ode(const Ode& other)
    : t0_(other.t0_),
      y0_(other.y0_),
      name_(other.name_),
      func_(other.func_),
      root_finder_(other.root_finder_) {
        //No need to verify dimensions here as they are copied from a valid Ode
}

Ode& Ode::operator=(const Ode& other) {
    if (this != &other) {
        t0_ = other.t0_;
        y0_ = other.y0_;
        name_ = other.name_;
        func_ = other.func_;
        root_finder_ = other.root_finder_;
        func_->Eval(t0_, y0_); // verify dimensions after assignment
    }
    return *this;
}

DynamicTensor Ode::Evaluate(double t, const DynamicTensor& y) const {
    return func_->Eval(t, y);
}

Ode::Ode(const Reader& reader) {
    const OdeRawData& raw_data = reader.GetRawData();
    t0_ = raw_data.time_params.t0;
    y0_ = raw_data.y0;
    root_finder_ = raw_data.root_finder;
    func_ = raw_data.function;
    name_ = reader.GetFileName();

    func_->Eval(t0_, y0_); // verify dimensions at construction
}

Ode::Ode(const OdeRawData& raw)
    : t0_(raw.time_params.t0),
      y0_(raw.y0),
      name_("ODE"),
      root_finder_(raw.root_finder),   // or raw.root_finder if you add it
      func_(raw.function)
{
    func_->Eval(t0_, y0_); // verify dimensions at construction
}

const std::string& Ode::GetName() const {return name_;}
const DynamicTensor& Ode::GetCondIn() const {return y0_;}
double Ode::GetTimeIn() const {return t0_;}
const Function& Ode::GetFunction() const {return *func_;}
const std::shared_ptr<RootFinder>& Ode::GetRootFinder() const {return root_finder_;}

void Ode::SetT0(double time) {t0_ = time;}

void Ode::SetY0(const DynamicTensor& y0) {
    y0_ = y0;
    func_->Eval(t0_, y0_); // verify dimensions after setting new initial condition
}
void Ode::SetY0(double in_y0) {
    y0_ = DynamicTensor(in_y0);
    func_->Eval(t0_, y0_); // verify dimensions after setting new initial condition
}

void Ode::SetName(const std::string& name) {name_ = name;}

void Ode::SetFunction(std::shared_ptr<const Function> func) {
    func_ = func;
    func_->Eval(t0_, y0_); // verify dimensions after setting new function
}
void Ode::SetRootFinder(std::shared_ptr<RootFinder> root_finder) {root_finder_ = root_finder;}