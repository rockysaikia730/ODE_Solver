#ifndef ODE_RAW_DATA_H_
#define ODE_RAW_DATA_H_

#include "dynamic_tensor.h"
#include "solver_methods.h"
#include <function.h>
#include <memory>

/**
 * @struct OdeRawData
 * @brief A structure holding all data read from the input source in a form
 * compatible with ODE and Solver classes.
 * This struct serves as an intermediate container for raw data
*/
struct OdeRawData {
    /**
     * @brief Default constructor.
     */
    OdeRawData() = default;

    /**
     * @brief Default Destructor.
     */
    ~OdeRawData() = default;

    /**
     * @brief Copy Constructor.
     */
    OdeRawData(const OdeRawData& other);

    /**
     * @brief Copy Assignment Operator.
     */
    OdeRawData& operator=(const OdeRawData& other);

    /**
     * @struct time_params
     * @brief Structure to hold time-related parameters for ODE solving.
     */
    struct time_params {
        double t0;
        double t_final = 0.0;
        double step_size = 0.01;
        size_t number_of_steps;
    };

    /**
     * @struct solver_params
     * @brief Structure to hold solver-related parameters for ODE solving.
     */
    struct solver_params {
        SolverMethod method = SolverMethod::kRungeKutta4;
        size_t max_iterations = 1000;
        double tolerance = 1e-6;
    };

    /**
     * @brief Initial condition tensor.
     */
    DynamicTensor y0;

    /**
     * @brief Pointer to the function defining the ODE.
     */    
    std::unique_ptr<Function> function; // Or should it be a shared_ptr?

    /**
     * @brief Time-related parameters.
     */
    time_params time_params;

    /**
     * @brief Solver-related parameters.
     */
    solver_params solver_params;

};

#endif // ODE_RAW_DATA_H_
