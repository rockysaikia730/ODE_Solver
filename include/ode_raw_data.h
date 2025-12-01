#ifndef ODE_RAW_DATA_H_
#define ODE_RAW_DATA_H_

#include <string>
#include "dynamic_tensor.h"
#include "solver_methods.h"

/**
 * @struct OdeRawData
 * @brief Raw, unparsed data extracted by a Reader. 
 * All values are strings or simple primitives.
 */
struct OdeRawData {

    /// @brief Initial time as string
    std::string t_init_string = "";

    /// @brief Final time as string
    std::string t_final_string = "";

    /// @brief Initial condition tensor as string
    std::string y0_string = "";

    /// @brief Dimension (optional), can be inferred from y0
    std::string input_dim_string = "";

    /// @brief Step size and number of steps as strings
    std::string step_size_string = "";
    std::string number_of_steps_string = "";

    /// @brief Tolerance of numerical similarity
    std::string tolerance_string = "";


    /// @brief Maximum number of iterations as string
    std::string max_iterations_string = "";

    /// @brief RHS function selection as string
    std::string rhs_function_string = "";

    /// @brief Solver method selection as string
    std::string solver_method_string = "";
};

#endif // ODE_RAW_DATA_H_
