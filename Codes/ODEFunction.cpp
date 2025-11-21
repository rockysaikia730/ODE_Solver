//
// Created by andra on 21/11/2025.
//
#include <vector>
#include <functional>
#include <iostream>
#include <complex>
#include "VectorOutput.h"
#include "ODEFunction.h"
#include "ODE.h"
using namespace std;

template<typename T>
ODEFunction<T>::ODEFunction(double t0, const State& y0, const Function& f, const std::string& name): ODE<T>(t0, y0, name), rhs(std::move(f))
{}

template<typename T>
ODEFunction<T>::ODEFunction(double t0, const T& y0, const Function& f, const std::string& name): ODE<T>(t0, y0, name), rhs(std::move(f))
{}


template<typename T>
ODEFunction<T>::State ODEFunction<T>::evaluate(double t, const State& y) const {
    return rhs(t,y);
}

template<typename T>
ODEFunction<T>::State ODEFunction<T>::evaluate(double t, const T& y) const {
    return rhs(t, std::vector<T>{y});
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Test functions
/// Move to test folder later
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functor example
struct ScaleFunctor {
    std::vector<double> operator()(double t, const std::vector<double>& y) const {
        std::vector<double> out = y;
        for (auto& x : out) x *= t;
        return out;
    }
};

int main() {
    cout << "===========================================" << endl;
    cout << "Test 1: Identity function" << endl;
    auto identity = [](double t, const vector<double>& y) {
        return y;  // just return the same state
    };

    ODEFunction<double> ode1(0.0, 1.0, identity, "identity");
    cout <<"function: "<< ode1.getName() << "; t0 = " << ode1.getTimeIn() << "; y0 = " << ode1.getCondIn() << endl;

    vector<double> state = {2.0, 3.0};
    auto result = ode1.evaluate(0.5, state);
    cout << "input: "<< state<< "; result: " << result << endl;

    cout << "===========================================" << endl;
    cout << "Test 2: Linear function" << endl;
    auto linear = [](double t, const std::vector<double>& y) {
        std::vector<double> out = y;
        for (auto& x : out) x += t;
        return out;
    };
    ODEFunction<double> ode2(0.0, 0.0, linear, "linear growth");
    cout <<"function: "<< ode2.getName() << "; t0 = " << ode2.getTimeIn() << "; y0 = " << ode2.getCondIn() << endl;

    std::vector<double> s = {1.0, 2.0};
    auto res2 = ode2.evaluate(1.0, s);
    cout << "input: "<< s << "; result: " << res2 << endl;

    cout << "===========================================" << endl;
    cout << "Test 3: scaling" << endl;
    auto scale = [](double t, const std::vector<int>& y) {
        std::vector<int> out = y;
        for (auto& x : out) x *= static_cast<int>(t);
        return out;
    };
    ODEFunction<int> ode3(1.0, 2.0, scale, "scaling");
    cout <<"function: "<< ode3.getName() << "; t0 = " << ode3.getTimeIn() << "; y0 = " << ode3.getCondIn() << endl;

    std::vector<int> s3 = {1, 2, 3};
    auto res3 = ode3.evaluate(3.0, s3);
    cout << "input: "<< s3 << "; result: " << res3 << endl;

    cout << "===========================================" << endl;
    cout << "Test 4: Empty function" << endl;
    auto emptyCheck = [](double t, const std::vector<double>& y) {
        return std::vector<double>{};
    };
    ODEFunction<double> ode4(0.0, 0.0, emptyCheck, "empty");
    cout <<"function: "<< ode4.getName() << "; t0 = " << ode4.getTimeIn() << "; y0 = " << ode4.getCondIn() << endl;
    std::vector<double> s4 = {};
    auto res4 = ode4.evaluate(0.0, s4);
    cout << "input: "<< s4 << "; result: " << res4 << endl;

    cout << "===========================================" << endl;
    cout << "Test 5: Functor test" << endl;
    ScaleFunctor functor;
    ODEFunction<double> ode5(1.0, 1.0, functor, "ScaleFunctor");
    cout <<"function: "<< ode5.getName() << "; t0 = " << ode5.getTimeIn() << "; y0 = " << ode5.getCondIn() << endl;
    std::vector<double> s5 = {2.0, 3.0};
    auto res5 = ode5.evaluate(2.0, s5);
    cout << "input: "<< s5 << "; result: " << res5 << endl;

    cout << "===========================================" << endl;
    cout << "Test 6: 1D functions" << endl;

    // 1D lambdas wrapped to match std::vector interface
    auto linear1D = [](double t, const vector<double>& y) {
        return vector<double>{ y[0] + t };
    };

    auto scale1D = [](double t, const vector<int>& y) {
        return vector<int>{ y[0] * static_cast<int>(t) };
    };

    auto identity1D = [](double t, const vector<double>& y) {
        return y;  // just return the same state
    };

    auto emptyCheck1D = [](double t, const vector<double>& y) {
        return vector<double>{ 0.0 };
    };

    // Example usage
    ODEFunction<double> f1(0.0, 1.0, linear1D, "linear1D");
    ODEFunction<int> f2(0.0, 2, scale1D, "scale1D");
    ODEFunction<double> f3(0.0, 5.0, identity1D, "identity1D");
    ODEFunction<double> f4(0.0, 0.0, emptyCheck1D, "emptyCheck1D");

    cout << "linear1D at t=2: " << f1.evaluate(2.0, 1.0)[0] << endl;
    cout << "scale1D at t=3: " << f2.evaluate(3.0, 2)[0] << endl;
    cout << "identity1D at t=10: " << f3.evaluate(10.0, 5.0)[0] << endl;
    cout << "emptyCheck1D at t=5: " << f4.evaluate(5.0, 0.0)[0] << endl;

    return 0;
}

