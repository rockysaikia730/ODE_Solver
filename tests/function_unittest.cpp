#include "gtest/gtest.h"
#include "function.h"
#include "dynamic_tensor.h"
#include <memory>


class MyFunc : public Function {
public:
    DynamicTensor Eval(double t, const DynamicTensor& y) const override {
        return y * 2.0 + t * t * 5;
    };
    std::unique_ptr<Function> Clone() const override {
        return std::make_unique<MyFunc>(*this);
    };
};

TEST(Function, Evaluation) {
    MyFunc func;

    double t = 10;
    DynamicTensor t_input = DynamicTensor(5);
    DynamicTensor t_output = func.Eval(t, t_input);

    EXPECT_EQ(t_output.at<double>({0}) , 2*5.0 + t*t*5);
}

TEST(Function, Gradient) {
    MyFunc func;

    double t = 3;
    DynamicTensor t_input = DynamicTensor(5);
    DynamicTensor t_grad = func.Grad(t, t_input);

    EXPECT_NEAR(t_grad.at<double>({0}) , 2.0, 0.1);
}