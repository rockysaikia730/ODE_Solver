#ifndef ODE_RAW_DATA_H_
#define ODE_RAW_DATA_H_

#include "dynamic_tensor.h"
#include "solver_methods.h"
#include "function.h"
#include "root_finder.h"
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
     * The default implementation performs adequate member-wise copying.
     */
    OdeRawData(const OdeRawData& other) = default;

    /**
     * @brief Copy Assignment Operator.
     * The default implementation performs adequate member-wise copying.
     */
    OdeRawData& operator=(const OdeRawData& other) = default;

    /**
     * @struct time_params
     * @brief Structure to hold time-related parameters for ODE solving.
     */
    struct time_params {
        /**
         *  @brief Initial time. 
        */
        double t0;

        /**
         *  @brief Final time. 
        */
        double t_final = 0.0;
        
        /**
        *@brief Time step size. 
        */
        double step_size = 0.01;

        /**
         * @brief Number of time steps.
         */
        size_t number_of_steps;
    };

    /**
     * @struct solver_params
     * @brief Structure to hold solver-related parameters for ODE solving.
     */
    struct solver_params {

        /**
         * @brief The numerical solver method to be used.
         */
        SolverMethod method = SolverMethod::kRungeKutta4;

        /**
         * @brief Maximum number of iterations for iterative solvers.
         */
        size_t max_iterations = 1000;

        /**
         * @brief Tolerance for iterative solvers.
         */
        double tolerance = 1e-6;
    };
    /**
     * @struct function_params
     * @brief Structure to hold function-related parameters for ODE solving.
     */
    struct function_params {
        /** 
         * @brief Vector of strings representing the function expressions.
         */
        std::vector<std::string> function_expressions;
        
        /** 
         * @brief Shape of the function output tensor.
         */
        std::vector<size_t> function_shape;
        
        /** 
         * @brief Vector of strings representing the derivative expressions.
         */
        std::vector<std::string> derivative_expressions;
        
        /** 
         * @brief Shape of the derivative output tensor.
         */
        std::vector<size_t> derivative_shape;
    };
    
    /**
     * @brief Initial condition tensor.
     */
    DynamicTensor y0;

    /**
     * @brief Function-related parameters.
     */
    function_params function_params;

    /**
     * @brief Time-related parameters.
     */
    time_params time_params;

    /**
     * @brief Solver-related parameters.
     */
    solver_params solver_params;

    /**
     * @brief Pointer to the root finding strategy (optional).
     */
    std::shared_ptr<RootFinder> root_finder;

    /**
     * @brief Pointer to the function defining the ODE.
     */    
    std::shared_ptr<const Function> function;

};

#endif // ODE_RAW_DATA_H_
