#include <gtest/gtest.h>
#include "dynamic_tensor.h"
#include <vector>
#include <complex>

TEST(DynamicTensor, Initialization1) {
    DynamicTensor t1;
    EXPECT_EQ(t1.rank(), 0);
    EXPECT_EQ(t1.size(), 0);
    EXPECT_EQ(t1.get_shape(), std::vector<size_t>{});
    EXPECT_EQ(t1.IsComplex(), false);
}

TEST(DynamicTensor, InitializationDouble) {
    DynamicTensor t1(5);
    EXPECT_EQ(t1.rank(), 1);
    EXPECT_EQ(t1.size(), 1);
    EXPECT_EQ(t1.get_shape(), std::vector<size_t>{1});
    EXPECT_EQ(t1.IsComplex(), false);
    EXPECT_EQ(t1.at<double>({0}), 5.0);
}

TEST(DynamicTensor, InitializationComplex) {
    using complex = std::complex<double>;
    complex testcomplex(1.0,5.0);
    DynamicTensor t1(testcomplex);
    EXPECT_EQ(t1.rank(), 1);
    EXPECT_EQ(t1.size(), 1);
    EXPECT_EQ(t1.get_shape(), std::vector<size_t>{1});
    EXPECT_EQ(t1.IsComplex(), true);
    EXPECT_EQ(t1.at<complex>({0}), testcomplex);
}

TEST(DynamicTensor, InitializationShapeDouble) {
    std::vector<size_t> shape = {5,2,3};
    double init_val = 6.0;

    DynamicTensor t1(shape, init_val);
    EXPECT_EQ(t1.rank(), 3);
    EXPECT_EQ(t1.size(), 30);
    EXPECT_EQ(t1.get_shape(), shape);
    EXPECT_EQ(t1.IsComplex(), false);
    for(auto it = t1.begin<double>(); it != t1.end<double>(); it++) EXPECT_EQ(*it, init_val);
}

TEST(DynamicTensor, InitializationShapeComplex) {
    using complex = std::complex<double>;
    std::vector<size_t> shape = {5,2,3};
    complex init_val = {6.0, 5.0};

    DynamicTensor t1(shape, init_val);
    EXPECT_EQ(t1.rank(), 3);
    EXPECT_EQ(t1.size(), 30);
    EXPECT_EQ(t1.get_shape(), shape);
    EXPECT_EQ(t1.IsComplex(), true);
    for(auto it = t1.begin<complex>(); it != t1.end<complex>(); it++) EXPECT_EQ(*it, init_val);
}


template<typename T>
void CheckTensorOperation(const DynamicTensor& t, T val) {
    for(auto it = t.begin<T>(); it != t.end<T>(); it++) {
        EXPECT_EQ(*it, val);
    }
}

TEST(DynamicTensor, TensorOperationsDouble) {
    std::vector<size_t> shape1 = {2,5,3};
    double val1 = 5.0;
    DynamicTensor t1(shape1, val1);

    std::vector<size_t> shape2 = {2,5,3};
    double val2 = 9.0;
    DynamicTensor t2(shape2, val2);

    CheckTensorOperation<double>(t1+t2, val1+val2);
    CheckTensorOperation<double>(t1-t2, val1-val2);
    CheckTensorOperation<double>(t1*t2, val1*val2);
}

TEST(DynamicTensor, TensorOperationsComplex) {
    using complex = std::complex<double>;

    std::vector<size_t> shape1 = {2,5,3};
    complex val1 = {6.0, 1.0};
    DynamicTensor t1(shape1, val1);

    std::vector<size_t> shape2 = {2,5,3};
    complex val2 = {1.0, 9.0};
    DynamicTensor t2(shape2, val2);

   CheckTensorOperation<complex>(t1+t2, val1+val2);
   CheckTensorOperation<complex>(t1-t2, val1-val2);
   CheckTensorOperation<complex>(t1*t2, val1*val2);
}

TEST(DynamicTensor, TensorScalarOperationsDouble) {
    double v1 = 5.5, v2 = 9.9, factor = 2.0, v0 = 1.0;

    DynamicTensor t1({2, 2}, v0); 
    t1.at<double>({0, 0}) = v1;
    t1.at<double>({1, 1}) = v2;
    
    DynamicTensor t3 = t1 * factor; 

    EXPECT_EQ(t1.at<double>({0, 0}), v1);
    EXPECT_EQ(t1.at<double>({1, 1}), v2);
    EXPECT_EQ(t1.at<double>({0, 1}), v0);
    EXPECT_EQ(t1.at<double>({1, 0}), v0);

    EXPECT_EQ(t3.at<double>({1, 1}), v2 * factor);
    EXPECT_EQ(t3.at<double>({0, 0}), v1 * factor);
    EXPECT_EQ(t3.at<double>({0, 1}), v0 * factor);
    EXPECT_EQ(t3.at<double>({1, 0}), v0 * factor);
}

TEST(DynamicTensor, TensorScalarOperationsComplex) {
    using complex = std::complex<double>;
    complex v1 = {5.5, 3.4}, v2 = {9.9, 6.8}, v0 = {1.0,0.0};
    double factor = 2.0;

    DynamicTensor t1({2, 2}, v0); 
    t1.at<complex>({0, 0}) = v1;
    t1.at<complex>({1, 1}) = v2;
    
    DynamicTensor t3 = t1 * factor; 

    EXPECT_EQ(t1.at<complex>({0, 0}), v1);
    EXPECT_EQ(t1.at<complex>({1, 1}), v2);
    EXPECT_EQ(t1.at<complex>({0, 1}), v0);
    EXPECT_EQ(t1.at<complex>({1, 0}), v0);

    EXPECT_EQ(t3.at<complex>({1, 1}), v2 * factor);
    EXPECT_EQ(t3.at<complex>({0, 0}), v1 * factor);
    EXPECT_EQ(t3.at<complex>({0, 1}), v0 * factor);
    EXPECT_EQ(t3.at<complex>({1, 0}), v0 * factor);
}