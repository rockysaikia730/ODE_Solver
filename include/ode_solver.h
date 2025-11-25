#ifndef CODES_ODE_SOLVER_H
#define CODES_ODE_SOLVER_H

#include <memory>
#include <vector>
#include <cmath>
#include "ODE.h"

/**
 * @class OdeSolver
 * @brief Abstract base class for Ordinary Differential Equation (ODE) solvers.
 * * This class provides the framework for solving ODEs numerically using specific
 * integration methods. 
 * * @tparam T The data type used for calculations (usually double).
 */
template <typename T>
class OdeSolver {
public:
    using State = std::vector<T>;
    virtual ~OdeSolver() = default;
    virtual void Step(double current_time, const State& current_state) = 0;

    void SetStartTime(double start_time) {start_time_ = start_time;}
    void SetEndTime(double end_time) {end_time_ = end_time;}
    void SetStepSize(double step_size) {
        step_size_ = step_size;
        num_of_steps_ = std::ceil((end_time_ - start_time_)/step_size_);
    }
    void SetNumOfSteps(int num_of_steps) {
        num_of_steps_ = num_of_steps;
        step_size_ = (end_time_ - start_time_)/num_of_steps_;
    }

    double GetStartTime() const {return start_time_;}
    double GetEndTime() const {return end_time_;}
    double GetStepSize() const {return step_size_;}
    double GetNumOfSteps() const {return num_of_steps_;}
    const std::vector<State>& GetSolution() const {return solutions_;}
    const std::vector<double>& GetTimeHistory() const {return times_;}

    void Solve() {
        double current_time = start_time_;
        State current_state = ode_.getCondIn();

        solutions_.clear();
        solutions_.push_back(current_state);
        times_.clear();
        times_.push_back(current_time);

        for(long long i = 0; i < num_of_steps_; i++) {
            current_state = Step(current_time, current_state);
            current_time += step_size_;

            solutions_.push_back(current_state);
            times_.push_back(current_time);
        }
    }

    OdeSolver(const ODE<T>& ode, double start_time, double end_time,  int num_of_steps = 1)
        : start_time_(start_time),
          end_time_(end_time),
          num_of_steps_(num_of_steps),
          ode_(ode)   
    {
        if(num_of_steps_ > 0) {
            step_size_ = (end_time_ - start_time_)/num_of_steps_;
        }
        else {
            step_size_ = 0.01;
            num_of_steps_ = std::ceil((end_time_ - start_time_)/step_size_);
        }
    }
protected:
    double start_time_;
    double end_time_;
    int num_of_steps_;
    double step_size_;
    const ODE<T>& ode_;
    std::vector<State> solutions_;
    std::vector<double> times_;
};
#endif
