#include "ode.h"
#include "dynamic_tensor.h"
#include "function.h"
#include "root_finder.h"
#include "ode_raw_data.h"
#include "reader.h"
#include <vector>
#include <string>
#include <memory>


Ode::Ode(double in_time, const DynamicTensor& in_y0, const std::string& in_name,
         std::unique_ptr<Function> func, std::unique_ptr<Function> derivative, std::shared_ptr<RootFinder> root_finder)
    : t0_(in_time),
      y0_(in_y0),
      name_(in_name),
      func_(std::move(func)),
      derivative_(std::move(derivative)),
      root_finder_(std::move(root_finder)) {
    
        VerifyDimensions();
    }

Ode::Ode(double in_time, const double& in_y0, const std::string& in_name,
         std::unique_ptr<Function> func, std::unique_ptr<Function> derivative, std::shared_ptr<RootFinder> root_finder)
    : t0_(in_time),
      y0_(in_y0),
      name_(in_name),
      func_(std::move(func)),
      derivative_(std::move(derivative)),
      root_finder_(std::move(root_finder)) {
        VerifyDimensions();
      }

Ode::~Ode() {}

Ode::Ode(const Ode& other)
    : t0_(other.t0_),
      y0_(other.y0_),
      name_(other.name_),
      root_finder_(other.root_finder_) {
    if (other.func_ != nullptr) {
        func_ = other.func_->Clone();
    } else {
        func_ = nullptr;
    }
    if (other.derivative_ != nullptr) {
        derivative_ = other.derivative_->Clone();
    } else {
        derivative_ = nullptr;
    }
    VerifyDimensions();
}

Ode& Ode::operator=(const Ode& other) {
    if (this != &other) {
        t0_ = other.t0_;
        y0_ = other.y0_;
        name_ = other.name_;
        if (other.func_ != nullptr) {
            func_ = other.func_->Clone();
        } else {
            func_ = nullptr;
        }
        if (other.derivative_ != nullptr) {
            derivative_ = other.derivative_->Clone();
        } else {
            derivative_ = nullptr;
        }
        root_finder_ = other.root_finder_;
        VerifyDimensions();
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

    if (raw_data.function != nullptr) {
        func_ = raw_data.function->Clone();
    } else {
        func_ = nullptr;
    }
    name_ = reader.GetFileName();
    if (raw_data.derivative != nullptr) {
        derivative_ = raw_data.derivative->Clone();
    } else {
        derivative_ = nullptr;
    }
    VerifyDimensions();
}

Ode::Ode(const OdeRawData& raw)
    : t0_(raw.time_params.t0),
      y0_(raw.y0),
      name_("ODE"),
      root_finder_(raw.root_finder)   // or raw.root_finder if you add it
{
    if (raw.function != nullptr){
            func_ = raw.function->Clone();
        } else {
            func_ = nullptr;
        }
    if (raw.derivative != nullptr){
            derivative_ = raw.derivative->Clone();
        } else {
            derivative_ = nullptr;
        }
    VerifyDimensions();
}

const std::string& Ode::GetName() const {return name_;}
const DynamicTensor& Ode::GetCondIn() const {return y0_;}
double Ode::GetTimeIn() const {return t0_;}
const Function& Ode::GetFunction() const {return *func_;}
const std::shared_ptr<RootFinder>& Ode::GetRootFinder() const {return root_finder_;}

void Ode::SetT0(double time) {t0_ = time;}

void Ode::SetY0(const DynamicTensor& y0) {y0_ = y0;}
void Ode::SetY0(double in_y0) {y0_ = DynamicTensor(in_y0);}

void Ode::SetName(const std::string& name) {name_ = name;}

void Ode::SetFunction(std::unique_ptr<Function> func) {func_ = std::move(func);}
void Ode::SetRootFinder(std::shared_ptr<RootFinder> root_finder) {root_finder_ = std::move(root_finder);}
void Ode::SetDerivative(std::unique_ptr<Function> derivative) {derivative_ = std::move(derivative);}

void Ode::VerifyDimensions() const {
    // Check that the dimensions of y0 match the output dimensions of func_ and derivative_
    if (func_ == nullptr) {
        throw std::runtime_error("Function is not set.");
    }
    const std::vector<size_t>& func_shape = func_->GetShape();
    if (func_shape != y0_.get_shape()) {
        throw std::runtime_error("Dimension mismatch between function output and initial condition y0.");
    }

    if (derivative_ != nullptr) {
        const std::vector<size_t>& deriv_shape = derivative_->GetShape();
        if (deriv_shape != y0_.get_shape()) {
            throw std::runtime_error("Dimension mismatch between derivative function output and initial condition y0.");
        }
    }
}