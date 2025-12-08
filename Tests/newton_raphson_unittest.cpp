#include "gtest/gtest.h"
#include "newton_raphson.h"
#include "function.h"
#include "dynamic_tensor.h"
#include <vector>

class SquaredRoot : public Function {
public:
    SquaredRoot(double num) 
    : num_(num) {};

    DynamicTensor Eval(double t, const DynamicTensor& y) const override {
        return y * y - num_;
    };
    std::unique_ptr<Function> Clone() const override {
        return std::make_unique<SquaredRoot>(*this);
    };

    const std::vector<size_t>& GetShape() const override {
        static std::vector<size_t> shape = {1};
        return shape;
    };

    void SetNum(double num) {num_ = num;}
private:
    double num_;
};


TEST(NewtonRaphson, Convergence) {
    SquaredRoot sqrt(16);

    NewtonRaphson solver;

    DynamicTensor solution = solver.FindRoot(sqrt, DynamicTensor(10), 0);
    EXPECT_EQ(solution.at<double>({0}), 4);

    sqrt.SetNum(25);
    solution = solver.FindRoot(sqrt, DynamicTensor(100), 0);
    EXPECT_EQ(solution.at<double>({0}), 5);
}