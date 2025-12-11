#include "backward_euler_solver.h"
#include "dynamic_tensor.h"
#include "ode.h"
#include "function.h"
#include "root_finder.h"
#include <cmath>
#include "gtest/gtest.h"

class RhsBE : public Function {
public:
    DynamicTensor Eval(double t, const DynamicTensor& y) const override {
        return -1.0 * y;
    }
    DynamicTensor Grad(double t, const DynamicTensor& y, double dy) const override {
        return DynamicTensor(-1.0);
    }
};


TEST(BackwardEuler, Initialisation) {
    auto rhs_ptr = std::make_shared<RhsBE>();
    Ode ode(0.0, 0.0, "-y", rhs_ptr);
    BackwardEuler be(ode);

    EXPECT_EQ(be.GetStartTime(), 0.0);
    EXPECT_EQ(be.GetCurrentTime(), 0.0);
    EXPECT_EQ(be.GetEndTime(), 0.0);
    EXPECT_EQ(be.GetStepSize(), 0.01);
}

TEST(BackwardEuler, InitialisationUserDefined) {
    auto rhs_ptr = std::make_shared<RhsBE>();
    Ode ode(0.0, 0.0, "-y", rhs_ptr);
    BackwardEuler be(ode, 0.5, 10);

    EXPECT_EQ(be.GetStartTime(), 0.0);
    EXPECT_EQ(be.GetCurrentTime(), 0.0);
    EXPECT_EQ(be.GetEndTime(), 10.0);
    EXPECT_EQ(be.GetStepSize(), 0.5);
}

TEST(BackwardEuler, Solve) {
    auto rhs_ptr = std::make_shared<RhsBE>();
    double tf = 0.05;
    
    Ode ode(0.0, 1.0, "-y", rhs_ptr); 
    BackwardEuler be(ode, 0.001, tf); 

    be.Solve();
    
    double solution_tf = exp(-tf); 
    EXPECT_NEAR(be.GetSolution().at<double>({0}), solution_tf, 1e-1);
    EXPECT_NEAR(be.GetCurrentTime(), tf, 1e-4); 
}

TEST(BackwardEuler, Step) {
    auto rhs_ptr = std::make_shared<RhsBE>();
    Ode ode(0.0, 1.0, "-y", rhs_ptr);

    double step_size = 1e-3;
    double t0 = ode.GetTimeIn();
    BackwardEuler be(ode, step_size);
    
    int iter = 10;
    for(int i=0; i<iter; i++) {
        be.Step();
    }

    double tf = t0 + iter * be.GetStepSize();
    double solution_tf = exp(-tf);
    EXPECT_NEAR(be.GetSolution().at<double>({0}), solution_tf, 1e-2);
    EXPECT_NEAR(be.GetCurrentTime(), tf, 1e-3);
}
