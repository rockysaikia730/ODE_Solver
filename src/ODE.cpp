#include "ode.h"
#include "dynamic_tensor.h"
#include "function.h"
#include "root_finder.h"
#include <vector>
#include <string>
#include <memory>


Ode::Ode(double in_time, const DynamicTensor& in_y0, const std::string& in_name,
         std::shared_ptr<Function> func, std::shared_ptr<RootFinder> root_finder)
    : t0_(in_time),
      y0_(in_y0),
      name_(in_name),
      func_(std::move(func)),
      root_finder_(std::move(root_finder)) {}

Ode::Ode(double in_time, const double& in_y0, const std::string& in_name,
         std::shared_ptr<Function> func, std::shared_ptr<RootFinder> root_finder)
    : t0_(in_time),
      y0_(in_y0),
      name_(in_name),
      func_(std::move(func)),
      root_finder_(std::move(root_finder)) {}

Ode::~Ode() {}

DynamicTensor Ode::Evaluate(double t, const DynamicTensor& y) const {
    return func_->Eval(t, y);
}

Ode::Ode(const Reader& reader) {
    OdeRawData raw_data = reader.GetRawData();
    t0_ = raw_data.time_params.t0;
    y0_ = raw_data.y0;
}
const std::string& Ode::GetName() const {return name_;}

const DynamicTensor& Ode::GetCondIn() const {return y0_;}

void Ode::SetT0(double time) {t0_ = time;}
void Ode::SetY0(const DynamicTensor& y0) {y0_ = y0;}
void Ode::SetY0(double in_y0) {y0_ = DynamicTensor(in_y0);}
void Ode::SetName(const std::string& name) {name_ = name;}
void Ode::SetFunction(std::shared_ptr<Function> func) {func_ = std::move(func);}
void Ode::SetRootFinder(std::shared_ptr<RootFinder> root_finder) {root_finder_ = std::move(root_finder);}