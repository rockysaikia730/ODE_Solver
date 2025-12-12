#include <gtest/gtest.h>
#include <fstream>
#include <string>

#include "output_csv.h"
#include "output_txt.h"
#include "runge_kutta_solver.h"
#include "parsed_function.h"
#include "dynamic_tensor.h"
#include "ode.h"

// All provided by ChatGPT based on the existing code structure


// Helper: read full file into a string
static std::string readFile(const std::string& fname) {
    std::ifstream f(fname);
    return std::string((std::istreambuf_iterator<char>(f)),
                       std::istreambuf_iterator<char>());
}

// testing simple CSV output
TEST(OutputTest, CsvOutput)
{
    // ODE: f = [y0, y1]
    auto f = std::make_unique<ParsedFunction>(
        std::vector<std::string>{"y0", "y1"},
        std::vector<size_t>{2}
    );

    DynamicTensor y0(std::vector<double>{1, 2}, {2});
    Ode ode(0.0, y0, "csv_test", std::move(f), nullptr);

    // Real solver
    RungeKutta solver(ode, 0.1, 0.0);
    solver.Reset(); // solution = y0, current_time = t0

    OutputCsv writer("data/test_out.csv", ';');
    writer.SetHeader({"H1", "H2", "H3"});
    writer.Write(solver);

    std::string contents = readFile("data/test_out.csv");
    // Check header
    EXPECT_NE(contents.find("H1;H2;H3"), std::string::npos);

    // Metadata
    EXPECT_NE(contents.find("t;0"), std::string::npos);
    EXPECT_NE(contents.find("step_size;0.1"), std::string::npos);

    // Tensor format (your exact format)
    EXPECT_NE(contents.find("y; [1, 2]"), std::string::npos)
        << "Tensor did not match expected CSV format: " << contents;
}

// testing simple TXT output
TEST(OutputTest, TxtOutput)
{
    auto f = std::make_unique<ParsedFunction>(
        std::vector<std::string>{"y0", "y1"},
        std::vector<size_t>{2}
    );

    DynamicTensor y0(std::vector<double>{1, 2}, {2});
    Ode ode(0.0, y0, "txt_test", std::move(f), nullptr);

    RungeKutta solver(ode, 0.1, 0.0);
    solver.Reset();

    OutputTxt writer("data/test_out.txt", ' ', '#');
    writer.SetHeader({"Header Line"});
    writer.Write(solver);

    std::string contents = readFile("data/test_out.txt");

    EXPECT_NE(contents.find("# Header Line"), std::string::npos);

    // Metadata
    EXPECT_NE(contents.find("t 0"), std::string::npos);

    // Tensor nested notation
    EXPECT_NE(contents.find("y  [1, 2]"), std::string::npos)
        << "TXT tensor format mismatch. Got:\n" << contents;
}

// test with actual function integration
TEST(OutputTest, ActualFunction_Sin)
{
    // f(t,y) = sin(t)
    auto f = std::make_unique<ParsedFunction>(
        std::vector<std::string>{"sin(t)"}, std::vector<size_t>{1}
    );

    DynamicTensor y0(std::vector<double>{0}, {1});
    Ode ode(0.0, y0, "sin_test", std::move(f), nullptr);

    RungeKutta solver(ode, 0.1, 1.0);
    solver.Reset();
    solver.Solve(); // Perform RK4 integration

    OutputCsv writer("data/sin_out.csv", ';');
    writer.Write(solver);

    std::string contents = readFile("data/sin_out.csv");
    EXPECT_FALSE(contents.empty());

    // Expected numerical RK4 result at t=1 (this is because ∫0^1 sin(t) dt = 1 - cos(1) = 0.459698)
    std::string expected_prefix = "0.4596";  // safe prefix match

    EXPECT_NE(contents.find(expected_prefix), std::string::npos)
        << "Expected approx y(1) = 0.459698 not found in output:\n"
        << contents;
}
