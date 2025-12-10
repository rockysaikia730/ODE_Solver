#include "forward_euler_solver.h"
#include "ode.h"
#include "dynamic_tensor.h"
#include "function.h"
#include "gtest/gtest.h"
#include <cmath>


class Rhs : public Function {
public:
    DynamicTensor Eval(double t, const DynamicTensor& y) const override {
        return -1.0 * y;
    }
};

TEST(ForwardEuler, Initialisation) {
    auto rhs_ptr = std::make_shared<Rhs>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    ForwardEuler fe(exponential_ode);

    EXPECT_EQ(fe.GetStartTime(), 0.0);
    EXPECT_EQ(fe.GetCurrentTime(), 0.0); 
    EXPECT_EQ(fe.GetEndTime(), 0.0); 
    EXPECT_EQ(fe.GetStepSize(), 0.01);
}

TEST(ForwardEuler, InitialisationUserDefined) {
    auto rhs_ptr = std::make_shared<Rhs>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    ForwardEuler fe(exponential_ode, 0.5, 10); 

    EXPECT_EQ(fe.GetStartTime(), 0);
    EXPECT_EQ(fe.GetEndTime(), 10);
    EXPECT_EQ(fe.GetStepSize(), 0.5);
    EXPECT_EQ(fe.GetNumberOfSteps(), std::floor((10.0 - 0.0)/0.5));
}

TEST(ForwardEuler, Solve) {
    auto rhs_ptr = std::make_shared<Rhs>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    ForwardEuler fe(exponential_ode, 0.01, 1);

    fe.Solve();
    EXPECT_NEAR(fe.GetSolution().at<double>({0}), exp(-1), 1e-2); 
    EXPECT_NEAR(fe.GetCurrentTime(), 1, 1e-4);
}

TEST(ForwardEuler, Step) {
    auto rhs_ptr = std::make_shared<Rhs>();

    double t0 = 0;
    Ode exponential_ode(t0, 1.0, "Exponential Decay", rhs_ptr);
    ForwardEuler fe(exponential_ode);

    int iter = 50; 
    double dt = fe.GetStepSize(); 
    
    for(int i=0; i<iter; i++) {
        fe.Step();
    }
    double tf = t0 + iter * dt;
    EXPECT_NEAR(fe.GetCurrentTime(), tf, 1e-4);
    EXPECT_NEAR(fe.GetSolution().at<double>({0}), exp(-tf), 1e-2); 
}
