#include "ode_solver.h"
#include "ode.h"
#include "dynamic_tensor.h"
#include <cmath>
#include <deque>
#include <stdexcept>

void OdeSolver::Reset() {
    solution_ =  ode_.GetCondIn();
    current_time_ = ode_.GetTimeIn();
}

OdeSolver::OdeSolver(const Ode& ode, int num_of_steps, double end_time)
    : ode_(ode),
      end_time_(end_time),
      start_time_(ode_.GetTimeIn())
      { 
        if(num_of_steps <= 0) {
          throw std::invalid_argument("Number of steps must be positive.");
        }
        if(end_time_ <= start_time_) {
          throw std::invalid_argument("Endtime must be greater than starttime.");
        }

        step_size_ = (end_time_ - start_time_)/num_of_steps;
        Reset();
      }

OdeSolver::OdeSolver(const Ode& ode, double step_size, double end_time)
    : ode_(ode),
      end_time_(end_time),
      start_time_(ode_.GetTimeIn()),
      step_size_(step_size)
      {
        if(end_time_ < 0) {
          throw std::invalid_argument("Endtime must be non-negative."); 
        }

        if(step_size_ <= 0) {
          throw std::invalid_argument("Step size must be positive."); 
        }

        Reset();
      }

void OdeSolver::Solve() {
    if(end_time_ <= start_time_) {
      throw std::invalid_argument("Endtime must be greater than starttime for Solve.");
    }
    Reset();
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
int OdeSolver::GetNumberOfSteps() const {return std::ceil((end_time_ - start_time_)/step_size_);}
const DynamicTensor& OdeSolver::GetSolution() const {return solution_;}
double OdeSolver::GetCurrentTime() const {return current_time_;}

void OdeSolver::SetEndTime(double end_time) {
  if(end_time_ < 0) {
    throw std::invalid_argument("Endtime must be non-negative."); 
  }
  end_time_ = end_time;
}
void OdeSolver::SetTimeSpan(double step_size) {
  if(end_time_ < 0) {
    throw std::invalid_argument("Endtime must be non-negative.");
  }
  step_size_ = step_size;
}
void OdeSolver::SetTimeSpan(int num_of_steps) {
  if(num_of_steps <= 0) {
    throw std::invalid_argument("Number of steps must be positive.");
  }
  step_size_ = (end_time_ - start_time_)/num_of_steps;
}