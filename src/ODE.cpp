#include "ode.h"
#include "dynamic_tensor.h"
#include "function.h"
#include "root_finder.h"
<<<<<<< HEAD
#include "ode_raw_data.h"
#include "reader.h"
#include <vector>
=======
>>>>>>> 7f4853e610a3b2d09cfc4d9d898052ca571c61ec
#include <string>
#include <memory>


Ode::Ode(double in_time, const DynamicTensor& in_y0, const std::string& in_name,
         std::unique_ptr<Function> func, std::shared_ptr<RootFinder> root_finder)
    : t0_(in_time),
      y0_(in_y0),
      name_(in_name),
      func_(std::move(func)),
      root_finder_(std::move(root_finder)) {}

Ode::Ode(double in_time, const double& in_y0, const std::string& in_name,
         std::unique_ptr<Function> func, std::shared_ptr<RootFinder> root_finder)
    : t0_(in_time),
      y0_(in_y0),
      name_(in_name),
      func_(std::move(func)),
      root_finder_(std::move(root_finder)) {}

Ode::~Ode() {}

Ode::Ode(const Ode& other)
    : t0_(other.t0_),
      y0_(other.y0_),
      name_(other.name_),
      func_(other.func_ ? other.func_->Clone() : nullptr),
      root_finder_(other.root_finder_) {}

Ode& Ode::operator=(const Ode& other) {
    if (this != &other) {
        t0_ = other.t0_;
        y0_ = other.y0_;
        name_ = other.name_;
        func_ = other.func_ ? other.func_->Clone() : nullptr;
        root_finder_ = other.root_finder_;
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
    func_ = std::unique_ptr<Function>(raw_data.function->Clone());
    name_ = reader.GetFileName();
}

Ode::Ode(const OdeRawData& raw)
    : t0_(raw.time_params.t0),
      y0_(raw.y0),
      name_("ODE"),
      func_(std::unique_ptr<Function>(raw.function->Clone())),
      root_finder_(raw.root_finder)   // or raw.root_finder if you add it
{}


const std::string& Ode::GetName() const {return name_;}
const DynamicTensor& Ode::GetCondIn() const {return y0_;}
double Ode::GetTimeIn() const {return t0_;}
const Function& Ode::GetFunction() const {return *func_;}
const std::shared_ptr<RootFinder>& Ode::GetRootFinder() const {return root_finder_;}

void Ode::SetT0(double time) {t0_ = time;}

void Ode::SetY0(const DynamicTensor& y0) {y0_ = y0;}
void Ode::SetY0(double in_y0) {y0_ = DynamicTensor(in_y0);}

void Ode::SetName(const std::string& name) {name_ = name;}
double Ode::GetTimeIn() const {return t0_;}

void Ode::SetFunction(std::unique_ptr<Function> func) {func_ = std::move(func);}
void Ode::SetRootFinder(std::shared_ptr<RootFinder> root_finder) {root_finder_ = std::move(root_finder);}
void Ode::SetDerivative(std::unique_ptr<Function> derivative) {derivative_ = std::move(derivative);}