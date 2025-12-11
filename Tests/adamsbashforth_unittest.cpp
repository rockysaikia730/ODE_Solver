#include "adamsbashforth_solver.h"
#include "ode.h"
#include "dynamic_tensor.h"
#include "gtest/gtest.h"

class RhsAB : public Function {
public:
    DynamicTensor Eval(double t, const DynamicTensor& y) const override {
        return -1.0 * y;
    }
};

TEST(AdamsBashforth, Initialisation) {
    auto rhs_ptr = std::make_shared<RhsAB>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    AdamsBashforth ab4(exponential_ode);

    EXPECT_EQ(ab4.GetStartTime(), 0.0);
    EXPECT_EQ(ab4.GetCurrentTime(), 0.01 * 3);
    EXPECT_EQ(ab4.GetEndTime(), 0.0);
    EXPECT_EQ(ab4.GetStepSize(), 0.01);
    EXPECT_EQ(ab4.GetOrder(), 4);
}


TEST(AdamsBashforth, InitialisationUserDefined) {
    auto rhs_ptr = std::make_shared<RhsAB>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    AdamsBashforth ab4(exponential_ode, 0.5, 10);

    EXPECT_EQ(ab4.GetStartTime(), 0);
    EXPECT_EQ(ab4.GetEndTime(), 10);
    EXPECT_EQ(ab4.GetStepSize(), 0.5);
    EXPECT_EQ(ab4.GetNumberOfSteps(), std::floor((10.0 - 0.0)/0.5));
}

TEST(AdamsBashforth, Solve) {
    auto rhs_ptr = std::make_shared<RhsAB>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    AdamsBashforth ab4(exponential_ode, 0.01, 1);

    ab4.Solve();
    EXPECT_NEAR(ab4.GetSolution().at<double>({0}), exp(-1), 1e-4);
    EXPECT_NEAR(ab4.GetCurrentTime(), 1, 1e-4);
}

TEST(AdamsBashforth, Step) {
    auto rhs_ptr = std::make_shared<RhsAB>();

    double t0 = 0;
    int order = 4;
    Ode exponential_ode(t0, 1.0, "Exponential Decay", rhs_ptr);
    AdamsBashforth ab4(exponential_ode);

    int iter = 100;
    for(int i=0; i<iter; i++) {
        ab4.Step();
    }

    double tf = t0 + (iter + order-1) * ab4.GetStepSize();
    EXPECT_NEAR(ab4.GetCurrentTime(), tf, 1e-4);
    EXPECT_NEAR(ab4.GetSolution().at<double>({0}), exp(-tf), 1e-4);
}
