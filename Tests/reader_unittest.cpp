#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <string>

#include "reader.h"
#include "reader_csv.h"
#include "reader_txt.h"
#include "parsed_function.h"
#include "dynamic_tensor.h"
#include "ode_raw_data.h"


using Complex = DynamicTensor::Complex;
void WriteTestFiles(const std::string& filename, const std::string& lines){
    std::ofstream f(filename);
    ASSERT_TRUE(f.good()) << "Failed to create test file: " << filename;
    f << lines;
    f.close();

}

// Test for CsvReader with simple real vector
TEST(ReaderTests, SimpleRealVectorCSV){
    WriteTestFiles("simple_vec.csv",
    "y; [1, 2, 3, 4]\n"
    "function; [y0, y1, y2, y3]\n"
    "t; 0\n"
    "tf; 1\n"
    "step_size; 0.1\n"
    );
    CsvReader reader("simple_vec.csv", ';', false);

    reader.Read();

    const OdeRawData& raw = reader.GetRawData();

    ASSERT_FALSE(raw.y0.IsComplex());
    EXPECT_EQ(raw.y0.get_shape(), std::vector<size_t>({4}));

    for (size_t i = 0; i < 4; ++i)
        EXPECT_DOUBLE_EQ(raw.y0.flat<double>(i), double(i + 1));
}

// Test for TxtReader with simple real vector
TEST(ReaderTests, SimpleRealVectorTXT)
{
    WriteTestFiles("simple_vec.txt",
        "y = [1, 2, 3, 4]\n"
        "function = [y0, y1, y2, y3]\n"
        "t = 0\n"
        "tf = 1\n"
        "step_size = 0.1\n"
    );

    TxtReader r("simple_vec.txt");
    r.Read();

    const auto& raw = r.GetRawData();

    ASSERT_FALSE(raw.y0.IsComplex());
    EXPECT_EQ(raw.y0.get_shape(), std::vector<size_t>({4}));

    for (size_t i = 0; i < 4; ++i)
        EXPECT_DOUBLE_EQ(raw.y0.flat<double>(i), double(i + 1));
}

// Test for CsvReader with complex vector
TEST(ReaderTests, ComplexVectorCSV)
{
    WriteTestFiles("complex_vec.csv",
        "y; [(1,2), (3,4), (5,6)]\n"
        "function; [y0, y1, y2]\n"
        "t; 0\n"
        "tf; 1\n"
        "step_size; 0.1\n"
    );

    CsvReader r("complex_vec.csv", ';', false);
    r.Read();

    const auto& raw = r.GetRawData();

    ASSERT_TRUE(raw.y0.IsComplex());
    EXPECT_EQ(raw.y0.get_shape(), std::vector<size_t>({3}));

    std::vector<Complex> expected = { Complex(1,2), Complex(3,4), Complex(5,6) };

    for (size_t i = 0; i < expected.size(); ++i) {
        Complex v = raw.y0.flat<Complex>(i);
        EXPECT_DOUBLE_EQ(v.real(), expected[i].real());
        EXPECT_DOUBLE_EQ(v.imag(), expected[i].imag());
    }
}

TEST(ReaderTests, ComplexVectorTXT)
{
    WriteTestFiles("complex_vec.txt",
        "y = [(1,2), (3,4), (5,6)]\n"
        "function = [y0, y1, y2]\n"
        "t = 0\n"
        "tf = 1\n"
        "step_size = 0.1\n"
    );

    TxtReader r("complex_vec.txt");
    r.Read();

    const auto& raw = r.GetRawData();

    ASSERT_TRUE(raw.y0.IsComplex());
    EXPECT_EQ(raw.y0.get_shape(), std::vector<size_t>({3}));
}

// Test for CsvReader with 2D complex tensor
TEST(ReaderTests, Tensor2DComplexCSV)
{
    WriteTestFiles("tensor2d.csv",
        "y; [[(1,2),(3,4)],[(5,6),(7,8)]]\n"
        "function; [[y0,y1],[y2,y3]]\n"
    );

    CsvReader r("tensor2d.csv", ';', false);
    r.Read();

    const auto& raw = r.GetRawData();

    ASSERT_TRUE(raw.y0.IsComplex());
    EXPECT_EQ(raw.y0.get_shape(), (std::vector<size_t>{2,2}));

    std::vector<Complex> expected = {
        Complex(1,2), Complex(3,4),
        Complex(5,6), Complex(7,8)
    };

    for (size_t i = 0; i < expected.size(); ++i) {
        Complex v = raw.y0.flat<Complex>(i);
        EXPECT_DOUBLE_EQ(v.real(), expected[i].real());
        EXPECT_DOUBLE_EQ(v.imag(), expected[i].imag());
    }
}

// Test for TxtReader with 2D complex tensor
TEST(ReaderTests, Tensor2DComplexTXT)
{
    WriteTestFiles("tensor2d.txt",
        "y = [[(1,2),(3,4)],[(5,6),(7,8)]]\n"
        "function = [[y0,y1],[y2,y3]]\n"
    );

    TxtReader r("tensor2d.txt");
    r.Read();

    const auto& raw = r.GetRawData();

    ASSERT_TRUE(raw.y0.IsComplex());
    EXPECT_EQ(raw.y0.get_shape(), (std::vector<size_t>{2,2}));
}

//Test for more complex functions
TEST(ReaderTests, FunctionEvaluation)
{
    WriteTestFiles("function_test.txt",
        "y = [1, 2]\n"
        "function = [y0 + y1, y0*y1 + t]\n"
        "t = 3\n"
    );

    TxtReader r("function_test.txt");
    r.Read();

    const auto& raw = r.GetRawData();
    ASSERT_TRUE(raw.function);

    auto* pf = dynamic_cast<const ParsedFunction*>(raw.function.get());
    ASSERT_NE(pf, nullptr);

    DynamicTensor result = pf->Eval(3.0, raw.y0);

    EXPECT_EQ(result.get_shape(), (std::vector<size_t>{2}));

    EXPECT_DOUBLE_EQ(result.flat<double>(0), 1+2);      // y0 + y1
    EXPECT_DOUBLE_EQ(result.flat<double>(1), 1*2 + 3);  // y0*y1 + t
}

//Derivative testing (if derivative expressions are provided)
TEST(ReaderTests, DerivativeParsingAndEvaluation)
{
    WriteTestFiles("derivative_test.txt",
        "y = [2, 3]\n"
        "function = [ y0^2 , t + y1 ]\n"
        "derivative = [[2*y0, 0], [0, 1]]\n"
        "t = 5\n"
    );

    TxtReader r("derivative_test.txt");
    r.Read();
    const auto& raw = r.GetRawData();

    ASSERT_TRUE(raw.function);

    auto* pf = dynamic_cast<const ParsedFunction*>(raw.function.get());
    ASSERT_NE(pf, nullptr);

    // -------------------------
    // Check that derivative expressions are parsed correctly
    EXPECT_EQ(raw.function_params.derivative_shape, (std::vector<size_t>{2,2}));
    EXPECT_EQ(raw.function_params.derivative_expressions.size(), 4); // flattened

    // Get function evaluation
    DynamicTensor F = pf->Eval(5.0, raw.y0);
    EXPECT_EQ(F.get_shape(), (std::vector<size_t>{2}));

    EXPECT_DOUBLE_EQ(F.flat<double>(0), 2*2);    // y0^2
    EXPECT_DOUBLE_EQ(F.flat<double>(1), 5 + 3);  // t + y1

    // -------------------------
    // Get the Jacobian
    DynamicTensor J = pf->Grad(5.0, raw.y0);

    ASSERT_FALSE(J.IsComplex());
    EXPECT_EQ(J.get_shape(), (std::vector<size_t>{2,2}));

    // Expected Jacobian:
    // [[2*y0,   0],
    //  [0,      1]]

    EXPECT_DOUBLE_EQ(J.flat<double>(0), 2 * 2.0); // 2*y0
    EXPECT_DOUBLE_EQ(J.flat<double>(1), 0.0);

    EXPECT_DOUBLE_EQ(J.flat<double>(2), 0.0);
    EXPECT_DOUBLE_EQ(J.flat<double>(3), 1.0);

    std::cout << "Jacobian:\n" << J << "\n";
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}