#include "adam_moulton_solver.h"
#include "dynamic_tensor.h"
#include "ode.h"
#include "gtest/gtest.h"

class RhsAM : public Function {
public:
    DynamicTensor Eval(double t, const DynamicTensor& y) const override {
        return -1.0 * y;
    }
};

TEST(AdamMoulton, Initialisation) {
    auto rhs_ptr = std::make_shared<RhsAM>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    AdamMoulton adam_moulton(exponential_ode);

    EXPECT_EQ(adam_moulton.GetStartTime(), 0.0);
    EXPECT_EQ(adam_moulton.GetCurrentTime(), 0.01 * (adam_moulton.GetOrder()-1));
    EXPECT_EQ(adam_moulton.GetEndTime(), 0.0);
    EXPECT_EQ(adam_moulton.GetStepSize(), 0.01);
    EXPECT_EQ(adam_moulton.GetOrder(), 4);
}

TEST(AdamMoulton, InitialisationUserDefined) {
    auto rhs_ptr = std::make_shared<RhsAM>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    AdamMoulton adam_moulton(exponential_ode, 0.5, 10);

    EXPECT_EQ(adam_moulton.GetStartTime(), 0);
    EXPECT_EQ(adam_moulton.GetEndTime(), 10);
    EXPECT_EQ(adam_moulton.GetStepSize(), 0.5);
    EXPECT_EQ(adam_moulton.GetNumberOfSteps(), std::floor((10.0 - 0.0)/0.5));
}

TEST(AdamMoulton, Solve) {
    auto rhs_ptr = std::make_shared<RhsAM>();
    Ode exponential_ode(0.0, 1.0, "Exponential Decay", rhs_ptr);
    AdamMoulton adam_moulton(exponential_ode, 0.01, 2);

    adam_moulton.Solve();
    EXPECT_NEAR(adam_moulton.GetSolution().at<double>({0}), exp(-2), 1e-4);
    EXPECT_NEAR(adam_moulton.GetCurrentTime(), 2, 1e-4);
}

TEST(AdamMoulton, Step) {
    auto rhs_ptr = std::make_shared<RhsAM>();

    double t0 = 0;
    Ode exponential_ode(t0, 1.0, "Exponential Decay", rhs_ptr);
    AdamMoulton adam_moulton(exponential_ode);

    int iter = 0;
    for(int i=0; i<iter; i++) {
        adam_moulton.Step();
    }

    double tf = t0 + (iter + adam_moulton.GetOrder()-1) * adam_moulton.GetStepSize();
    EXPECT_NEAR(adam_moulton.GetCurrentTime(), tf, 1e-4);
    EXPECT_NEAR(adam_moulton.GetSolution().at<double>({0}), exp(-tf), 1e-4);
}