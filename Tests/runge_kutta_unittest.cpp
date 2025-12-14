#include "gtest/gtest.h"

#include "ode.h"
#include "function.h"
#include "runge_kutta_solver.h"
#include <memory>
#include <cmath>

class Rhs : public Function {
public:
    DynamicTensor Eval(double t, const DynamicTensor& y) const {
        return -1.0 * y;
    }
};


TEST(RungeKutta, InitialisationDefault) {
    auto rhs_ptr = std::make_shared<Rhs>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    RungeKutta rk4(exponential_ode);
    
    EXPECT_EQ(rk4.GetStartTime(), 0);
    EXPECT_EQ(rk4.GetEndTime(), 0.0);
    EXPECT_EQ(rk4.GetStepSize(), 0.01);
    EXPECT_EQ(rk4.GetSolution().at<double>({0}), 1);
}

TEST(RungeKutta, InitialisationUserDefined) {
    auto rhs_ptr = std::make_shared<Rhs>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    RungeKutta rk4(exponential_ode, 0.5, 10);
    
    EXPECT_EQ(rk4.GetStartTime(), 0);
    EXPECT_EQ(rk4.GetEndTime(), 10);
    EXPECT_EQ(rk4.GetStepSize(), 0.5);
    EXPECT_EQ(rk4.GetNumberOfSteps(), std::floor((10.0 - 0.0)/0.5));
    EXPECT_EQ(rk4.GetSolution().at<double>({0}), 1);
}

TEST(RungeKutta, Solve) {
    auto rhs_ptr = std::make_shared<Rhs>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    RungeKutta rk4(exponential_ode, 0.01, 1);
    rk4.Solve();
    EXPECT_NEAR(rk4.GetSolution().at<double>({0}), exp(-1), 1e-4);
    EXPECT_NEAR(rk4.GetCurrentTime(), 1, 1e-4);
}

TEST(RungeKutta, Step) {
    auto rhs_ptr = std::make_shared<Rhs>();

    double t0 = 0;
    Ode exponential_ode(t0, 1.0, "Exponential Decay", rhs_ptr);
    RungeKutta rk4(exponential_ode);

    int iter = 1000;
    for(int i=0; i<iter; i++) {
        rk4.Step();
    }

    double tf = t0 + 1000 * rk4.GetStepSize();
    EXPECT_NEAR(rk4.GetCurrentTime(), tf, 1e-4);
    EXPECT_NEAR(rk4.GetSolution().at<double>({0}), exp(-tf), 1e-4);
}


