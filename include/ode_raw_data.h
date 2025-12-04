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

    struct time_params {
        double t0;
        double t_final = 0.0;
        double step_size = 0.01;
        size_t number_of_steps;
    };

    struct solver_params {
        SolverMethod method = SolverMethod::kRungeKutta4;
        size_t max_iterations = 1000;
        double tolerance = 1e-6;
    };

    DynamicTensor y0;
    std::shared_ptr<Function> func;
    time_params time_params;
    solver_params solver_params;

};

#endif // ODE_RAW_DATA_H_
