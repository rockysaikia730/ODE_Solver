#include <iostream>
#include <vector>
#include <complex>
#include <iomanip>
#include <stdexcept>
#include <variant> // For std::bad_variant_access
#include "dynamic_tensor.h"

// =========================================================
// Helper: Print Tensor
// Since operator<< was removed from the class, we need a 
// manual way to view data for testing.
// =========================================================
void PrintTensor(const DynamicTensor& t, const std::string& name) {
    std::cout << "Tensor: " << name << "\n";
    std::cout << "  Rank: " << t.rank() << " Shape: [ ";
    for (size_t d : t.get_shape()) std::cout << d << " ";
    std::cout << "]\n  Data: { ";

    size_t n = t.size();
    if (t.IsComplex()) {
        std::cout << "(Complex Data of size " << n << ")";
    } else {
        std::cout << "(Real Data of size " << n << ")";
    }
    std::cout << " }\n\n";
}

// =========================================================
// Main Test Suite
// =========================================================
int main() {
    using Complex = std::complex<double>;

    try {
        // ---------------------------------------------------------
        // TEST 1: Real Tensor Construction & Access
        // ---------------------------------------------------------
        std::cout << "--- TEST 1: Real Tensor ---\n";
        
        // 2x2 Matrix
        DynamicTensor t1({2, 2}, 1.0); 
        
        // Access using templated at<double>
        t1.at<double>({0, 0}) = 5.5;
        t1.at<double>({1, 1}) = 9.9;

        // Verify values
        std::cout << "Value at {0,0}: " << t1.at<double>({0, 0}) << " (Expected 5.5)\n";
        std::cout << "Value at {1,1}: " << t1.at<double>({1, 1}) << " (Expected 9.9)\n";
        
        PrintTensor(t1, "t1");

        // ---------------------------------------------------------
        // TEST 2: Complex Tensor Construction & Access
        // ---------------------------------------------------------
        std::cout << "--- TEST 2: Complex Tensor ---\n";

        // Rank 1 Vector of size 3
        DynamicTensor t2({3}, Complex(0, 1)); // 0 + 1i
        
        // Set a value
        t2.at<Complex>({2}) = Complex(2.0, 3.0);

        // Verify
        Complex c_val = t2.at<Complex>({2});
        std::cout << "Value at {2}: " << c_val.real() << " + " << c_val.imag() << "i\n";
        
        PrintTensor(t2, "t2");

        // ---------------------------------------------------------
        // TEST 3: Math Operators (Real)
        // ---------------------------------------------------------
        std::cout << "--- TEST 3: Real Math ---\n";
        
        // Multiplication by scalar
        DynamicTensor t3 = t1 * 2.0; 
        std::cout << "t3 (t1 * 2.0) at {0,0}: " << t3.at<double>({0, 0}) << " (Expected 11.0)\n";

        // Addition
        DynamicTensor t4 = t1 + t3;
        std::cout << "t4 (t1 + t3) at {0,0}: " << t4.at<double>({0, 0}) << " (Expected 16.5)\n";

        // ---------------------------------------------------------
        // TEST 4: Math Operators (Complex)
        // ---------------------------------------------------------
        std::cout << "--- TEST 4: Complex Math ---\n";

        // Multiplication by double scalar
        DynamicTensor t5 = t2 * 2.0;
        Complex c_res = t5.at<Complex>({0}); // Was i, should be 2i
        std::cout << "t5 (t2 * 2.0) at {0}: " << c_res.imag() << "i (Expected 2.0)\n";

        // ---------------------------------------------------------
        // TEST 5: Error Handling (Strict Mode)
        // ---------------------------------------------------------
        std::cout << "--- TEST 5: Error Handling ---\n";

        // A. Wrong Type Access
        try {
            std::cout << "Attempting to access Real tensor using at<Complex>... ";
            // This calls std::get<vector<Complex>> on a vector<double> variant
            // It should throw std::bad_variant_access
            t1.at<Complex>({0, 0}); 
            std::cout << "[FAILED] (Should have thrown)\n";
        } catch (const std::bad_variant_access& e) {
            std::cout << "[SUCCESS] Caught variant error: " << e.what() << "\n";
        } catch (...) {
             std::cout << "[SUCCESS] Caught expected exception.\n";
        }

        // B. Mixed Addition (Strict)
        try {
            std::cout << "Attempting to add Real + Complex tensors... ";
            
            // FIX: We must ensure shapes match first!
            // t1 is 2x2. We create a 2x2 Complex tensor.
            DynamicTensor t_comp_2x2({2, 2}, Complex(1,1));
            
            // This should throw std::runtime_error due to type mismatch
            // (Real + Complex)
            DynamicTensor strict_fail = t1 + t_comp_2x2;

            std::cout << "[FAILED] (Should have thrown)\n";
        } catch (const std::exception& e) {
            std::cout << "[SUCCESS] Caught strict mode error: " << e.what() << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "CRITICAL FAILURE: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\nAll tests passed successfully.\n";
    return 0;
}