#include <string>
#include <vector>
#include "../include/ode copy.h"
#include "../include/solver_methods.h"

Ode::Ode(const OdeRawData& raw_data) {

    if (!raw_data.t_init_string.empty()) {
        t_init_ = std::stod(raw_data.t_init_string);
    } else {
        t_init_ = kDefaultInitialTime;
    }
    
    if (!raw_data.tolerance_string.empty()) {
        tolerance_ = std::stod(raw_data.tolerance_string);
    } else {
        tolerance_ = kDefaultTolerance;
    }

    if (!raw_data.max_iterations_string.empty()) {
        max_iterations_ = std::stoi(raw_data.max_iterations_string);
    } else {
        max_iterations_ = kDefaultMaxIterations;
    }

    if (!raw_data.t_final_string.empty()) {
        t_final_ = std::stod(raw_data.t_final_string);
    } else {
        t_final_ = kDefaultFinalTime;
    }

    if (!raw_data.step_size_string.empty()) {
        step_size_ = std::stod(raw_data.step_size_string);
    } else {
        step_size_ = kDefaultStepSize;
    }

    if (!raw_data.number_of_steps_string.empty()) {
        number_of_steps_ =  std::stoi(raw_data.number_of_steps_string);
    } else {
        number_of_steps_ = kDefaultNumberOfSteps;
    }

    VerifyConditions(raw_data);
    ParseInitialConditionFromString(raw_data.y0_string);
    SetDimensionFromInitialCondition();
    ParseRhsFunctionFromString(raw_data.rhs_function_string);
    ParseSolverMethodFromString(raw_data.solver_method_string);

    // MISSING IMPLEMENTATION of parsers!
}


// Getters
double Ode::GetInitialTime() const {
    return t_init_;
}
double Ode::GetFinalTime() const {
    return t_final_;
}
DynamicTensor Ode::GetInitialCondition() const {
    return y0_;
}
std::vector<int> Ode::GetInputDimension() const {
    return input_dim_;
}
double Ode::GetStepSize() const {
    return step_size_;
}
int Ode::GetNumberOfSteps() const {
    return number_of_steps_;
}
double Ode::GetTolerance() const {
    return tolerance_;
}
int Ode::GetMaxIterations() const {
    return max_iterations_;
}
SolverMethod Ode::GetSolverMethod() const {
    return solver_method_enum_;
}

// Setters
void Ode::SetSolverMethod(SolverMethod method) {
    solver_method_enum_ = method;
}
void Ode::SetMaxIterations(int max_iterations) {
    max_iterations_ = max_iterations;
}
void Ode::SetTolerance(double tolerance) {
    tolerance_ = tolerance;
}
void Ode::SetStepSize(double step_size) {
    step_size_ = step_size;
}
void Ode::SetNumberOfSteps(int number_of_steps) {
    number_of_steps_ = number_of_steps;
}
void Ode::SetInitialCondition(const DynamicTensor& y0) {
    y0_ = y0;
}
void Ode::SetInitialTime(double t_init) {
    t_init_ = t_init;
}
void Ode::SetFinalTime(double t_final) {
    t_final_ = t_final;
}

DynamicTensor Ode::EvaluateRhs(double t, const DynamicTensor& y) const {
    // MISSING IMPLEMENTATION - depends on how RHS functions are represented
    return DynamicTensor(); // Placeholder return
}

// Private methods to parse from raw strings go here
void Ode::VerifyConditions(const OdeRawData& raw_data) {

    // Verify if T = t_final_ - t_init_ is compatible with step_size_ and number_of_steps_
    double T = t_final_ - t_init_;

    // check what was provided
    bool has_steps = (raw_data.number_of_steps_string.empty() == false);
    bool has_step_size = (raw_data.step_size_string.empty() == false);
    bool has_final_time = (raw_data.t_final_string.empty() == false);
    int specified_count = has_steps + has_step_size + has_final_time;

    if (specified_count < 2){
        if (has_final_time && !has_step_size && !has_steps){
            // Only final time provided: compute number of steps from default step size
            number_of_steps_ = static_cast<int>((t_final_ - t_init_) / step_size_);
        }
        else{
            // All other cases: recompute final time using what is provided and one of the defaults.
            t_final_ = t_init_ + number_of_steps_ * step_size_;
        }
    }
    else if (specified_count == 2)
    {
        if (!has_steps){
            // missing number of steps
            number_of_steps_ = static_cast<int>(T / step_size_);
        }   
        else if (!has_step_size){
            // missing step size
            step_size_ = T / static_cast<double>(number_of_steps_);
        }
        else if (!has_final_time){
            // missing final time
            t_final_ = t_init_ + number_of_steps_ * step_size_;
        }
    }
    else {
        // If all three provided, check for consistency
        double computed_T = number_of_steps_ * step_size_;
        if (std::abs(computed_T - T) > tolerance_){
            // handle inconsistency (throw, warn, clamp, etc.)
        }
    }
}

//Parsers
void Ode::ParseInitialConditionFromString(const std::string& y0_string) {
    // MISSING IMPLEMENTATION - depends on the format of y0_string
    // Example placeholder implementation:
    // y0_ = DynamicTensor::FromString(y0_string);
}

void Ode::ParseRhsFunctionFromString(const std::string& function_string) {
    // MISSING IMPLEMENTATION - depends on how functions are represented
    // Example placeholder implementation:
    // if (function_string == "some_function") {
    //     rhs_function_ = SomeFunctionImplementation;
    // }
}



void Ode::SetDimensionFromInitialCondition() {
    // MISSING IMPLEMENTATION - depends on how dimension is defined from y0_
    // Example placeholder implementation:
    // input_dim_ = y0_.get_shape(); // or some other logic
}