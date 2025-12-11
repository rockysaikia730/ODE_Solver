#include "backward_differentiation_solver.h"
#include "ode.h"
#include "dynamic_tensor.h"
#include "function.h"
#include "gtest/gtest.h"


class Rhs : public Function {
public:
    DynamicTensor Eval(double t, const DynamicTensor& y) const override {
        return -1.0 * DynamicTensor(y.get_shape(), t);
    }
};

TEST(BackwardDifferentiation, Initialisation) {
    auto rhs_ptr = std::make_shared<Rhs>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    Bdf bdf4(exponential_ode);

    EXPECT_EQ(bdf4.GetStartTime(), 0.0);
    EXPECT_EQ(bdf4.GetCurrentTime(), 0.01 * 3);
    EXPECT_EQ(bdf4.GetEndTime(), 0.0);
    EXPECT_EQ(bdf4.GetStepSize(), 0.01);
    EXPECT_EQ(bdf4.GetOrder(), 4);
}

TEST(BackwardDifferentiation, InitialisationUserDefined) {
    auto rhs_ptr = std::make_shared<Rhs>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    Bdf bdf4(exponential_ode, 0.5, 10);

    EXPECT_EQ(bdf4.GetStartTime(), 0);
    EXPECT_EQ(bdf4.GetEndTime(), 10);
    EXPECT_EQ(bdf4.GetStepSize(), 0.5);
    EXPECT_EQ(bdf4.GetNumberOfSteps(), std::floor((10.0 - 0.0)/0.5));
}


TEST(BackwardDifferentiation, Solve) {
    auto rhs_ptr = std::make_shared<Rhs>();

    double t0 = 0;
    int order = 4;
    double tf = 2;

    Ode exponential_ode(0.0, 0.0, "Exponential Decay", rhs_ptr);
    Bdf bdf4(exponential_ode, 0.01, tf);

    bdf4.Solve();
    
    double sol =  - tf*tf*0.5;
    EXPECT_NEAR(bdf4.GetSolution().at<double>({0}), sol, 1e-3);
    EXPECT_NEAR(bdf4.GetCurrentTime(), tf, 1e-4);
}

TEST(BackwardDifferentiation, Step) {
    auto rhs_ptr = std::make_shared<Rhs>();

    double t0 = 0;
    int order = 4;
    Ode exponential_ode(t0, 0.0, "Exponential Decay", rhs_ptr);
    Bdf bdf4(exponential_ode, 0.01, 0.0, order);

    int iter = 1000;
    for(int i=0; i<iter; i++) {
        bdf4.Step();
        std::cout << bdf4.GetSolution() << std::endl;
    }

    double tf = t0 + (iter + order-1) * bdf4.GetStepSize();
    double sol = exponential_ode.GetCondIn().at<double>({0}) - tf*tf*0.5;
    EXPECT_NEAR(bdf4.GetCurrentTime(), tf, 1e-4);
    EXPECT_NEAR(bdf4.GetSolution().at<double>({0}), sol, 1e-3);
}
