#include "ode_solver.h"
#include "ode.h"
#include "dynamic_tensor.h"
#include <cmath>
#include <deque>

void OdeSolver::Reset() {
    solution_ =  ode_.GetCondIn();
    current_time_ = ode_.GetTimeIn();
}

OdeSolver::OdeSolver(const Ode& ode, int num_of_steps, double end_time)
    : ode_(ode),
      end_time_(end_time),
      start_time_(ode_.GetTimeIn())
      {
        if(num_of_steps > 0) step_size_ = (end_time_ - start_time_)/num_of_steps;
        else step_size_ = 0.01;
        Reset();
      }

OdeSolver::OdeSolver(const Ode& ode, double step_size, double end_time)
    : ode_(ode),
      end_time_(end_time),
      start_time_(ode_.GetTimeIn()),
      step_size_(step_size)
      {
        Reset();
      }

void OdeSolver::Solve() {
    int num_of_steps = std::ceil((end_time_ -  current_time_)/step_size_);
    for(int i = 1; i <= num_of_steps; i++) {
        Step();
    }
}

void OdeSolver::Step() {
    solution_ = ComputeStep();
    current_time_ += step_size_;
}

double OdeSolver::GetStartTime() const {return start_time_;}
double OdeSolver::GetEndTime() const {return end_time_;}
double OdeSolver::GetStepSize() const {return step_size_;}
int OdeSolver::GetNumberOfSteps() const {return std::floor((end_time_ - start_time_)/step_size_);}
const DynamicTensor& OdeSolver::GetSolution() const {return solution_;}

void OdeSolver::SetEndTime(double end_time) {end_time_ = end_time;}
void OdeSolver::SetTimeSpan(double step_size) {step_size_ = step_size;}
void OdeSolver::SetTimeSpan(int num_of_steps) {step_size_ = (end_time_ - start_time_)/num_of_steps;}
