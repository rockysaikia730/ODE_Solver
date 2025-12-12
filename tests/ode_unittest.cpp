#include <gtest/gtest.h>
#include <fstream>

#include "ode.h"
#include "reader_csv.h"
#include "reader_txt.h"
#include "parsed_function.h"
#include "dynamic_tensor.h"

// simplifying
using Complex = DynamicTensor::Complex;

// Direct constructor from scalar initial condition
TEST(OdeTest, ScalarConstructor)
{
    auto f = std::make_unique<ParsedFunction>(
        std::vector<std::string>{"t + y0"},   // f(t,y)=t+y
        std::vector<size_t>{1}
    );

    Ode ode(0.5, 3.0, "scalar", std::move(f), nullptr);

    DynamicTensor result = ode.Evaluate(ode.GetTimeIn(), ode.GetCondIn());

    ASSERT_FALSE(result.IsComplex());
    EXPECT_EQ(result.get_shape(), std::vector<size_t>{1});
    EXPECT_DOUBLE_EQ(result.flat<double>(0), 3.0 + 0.5);
}

// Direct constructor from vector initial condition
TEST(OdeTest, VectorConstructor)
{
    DynamicTensor y0(std::vector<double>{1.0, 2.0}, {2});

    auto f = std::make_unique<ParsedFunction>(
        std::vector<std::string>{"y0 + y1", "t + y0"},   // f = [3, t+1]
        std::vector<size_t>{2}
    );

    Ode ode(1.0, y0, "vec", std::move(f), nullptr);

    DynamicTensor res = ode.Evaluate(ode.GetTimeIn(), ode.GetCondIn());
    ASSERT_FALSE(res.IsComplex());
    EXPECT_EQ(res.get_shape(), std::vector<size_t>{2});

    EXPECT_DOUBLE_EQ(res.flat<double>(0), 3.0); // y0 + y1 = 1+2
    EXPECT_DOUBLE_EQ(res.flat<double>(1), 1.0 + 1.0);
}

// Constructing from OdeRawData
TEST(OdeTest, ConstructFromRawData)
{
    OdeRawData raw;
    raw.y0 = DynamicTensor(std::vector<double>{5.0}, {1});
    raw.time_params.t0 = 2.0;

    raw.function = std::make_shared<ParsedFunction>(
        std::vector<std::string>{"y0 * t"}, std::vector<size_t>{1}
    );

    Ode ode(raw);
    DynamicTensor result = ode.Evaluate(ode.GetTimeIn(), ode.GetCondIn());

    ASSERT_FALSE(result.IsComplex());
    EXPECT_EQ(result.flat<double>(0), 5.0 * 2.0);
}

// Testing for copy constructor
TEST(OdeTest, CopyAndAssign)
{
    // original ODE
    auto f = std::make_unique<ParsedFunction>(
        std::vector<std::string>{"y0 + 1"}, std::vector<size_t>{1}
    );
    Ode original(0.0, 2.0, "copy_test", std::move(f), nullptr);

    // copy constructor
    Ode copyConstructed(original);
    EXPECT_EQ(copyConstructed.Evaluate(copyConstructed.GetTimeIn(),
                                       copyConstructed.GetCondIn())
                  .flat<double>(0),
              3.0);

    // copy assignment
    Ode assigned = original;
    EXPECT_EQ(assigned.Evaluate(assigned.GetTimeIn(),
                                assigned.GetCondIn())
                  .flat<double>(0),
              3.0);
}

// Building ODE from CSV reader
TEST(OdeTest, ConstructFromCsvReader)
{
    // build input file
    std::ofstream f("ode_test.csv");
    f << "y; [1.0, 2.0]\n";
    f << "t; 0\n";
    f << "tf; 1\n";
    f << "step_size; 0.1\n";
    f << "function; [y0 + y1, t + y0]\n";
    f.close();

    CsvReader r("ode_test.csv", ';', false);
    r.Read();

    Ode ode(r);

    DynamicTensor res = ode.Evaluate(ode.GetTimeIn(), ode.GetCondIn());

    EXPECT_EQ(res.get_shape(), std::vector<size_t>{2});
    EXPECT_DOUBLE_EQ(res.flat<double>(0), 3.0);
    EXPECT_DOUBLE_EQ(res.flat<double>(1), 0.0 + 1.0);
}

// Building ODE from TXT reader
TEST(OdeTest, ConstructFromTxtReader)
{
    std::ofstream f("ode_test.txt");
    f << "y = [1.0, 2.0]\n";
    f << "t = 0\n";
    f << "tf = 1\n";
    f << "step_size = 0.1\n";
    f << "function = [y0 + y1, t + y0]\n";
    f.close();

    TxtReader r("ode_test.txt");
    r.Read();

    Ode ode(r);

    DynamicTensor res = ode.Evaluate(ode.GetTimeIn(), ode.GetCondIn());

    EXPECT_EQ(res.get_shape(), std::vector<size_t>{2});
    EXPECT_DOUBLE_EQ(res.flat<double>(0), 3.0);
    EXPECT_DOUBLE_EQ(res.flat<double>(1), 1.0);
}

//  Check for derivative
TEST(OdeTest, DerivativeProvided)
{
    // f = [y0, y1]
    // df/dy = identity matrix
    std::vector<std::string> f_expr = {"y0", "y1"};
    std::vector<std::string> df_expr = {"1", "0", "0", "1"};
    std::vector<size_t> f_shape{2};
    std::vector<size_t> df_shape{2,2};

    auto f = std::make_unique<ParsedFunction>(f_expr, f_shape, df_expr, df_shape);

    DynamicTensor y0(std::vector<double>{3.0, -1.0}, {2});
    Ode ode(0.0, y0, "deriv_test", std::move(f), nullptr);

    DynamicTensor grad = ode.Grad(0.0, y0);

    EXPECT_EQ(grad.get_shape(), std::vector<size_t>({2,2}));

    EXPECT_DOUBLE_EQ(grad.flat<double>(0), 1.0);
    EXPECT_DOUBLE_EQ(grad.flat<double>(1), 0.0);
    EXPECT_DOUBLE_EQ(grad.flat<double>(2), 0.0);
    EXPECT_DOUBLE_EQ(grad.flat<double>(3), 1.0);
}
