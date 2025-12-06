#ifndef ODE_H_
#define ODE_H_

#include <string>
#include <vector>
#include "dynamic_tensor.h"
#include "solver_methods.h"
#include "ode_raw_data.h"

/**
 * @class ODE
 * @brief A class representing an ordinary differential equation (ODE).
 *
 * This class encapsulates the properties and methods related to ODEs,
 * including their initial conditions, the defining function, and the
 * necessary parameters for numerical solving, done using a Solver class.
 */
class Ode {
public:
    /**
     * @brief Constructor that initializes the ODE with given parameters.
     * @param raw_data An OdeRawData object containing raw data for initializing the ODE.
     * The constructor is responsible for parsing the raw data and setting up the ODE.
     */
    Ode(const OdeRawData& raw_data);

    /**
     * @brief Virtual destructor.
     */
    virtual ~Ode() = default;

    /**
     * @brief Get the initial time.
     * @return The initial time as a double.
     */
    double GetInitialTime() const;

    /**
     * @brief Get the final time.
     * @return The final time as a double.
     */
    double GetFinalTime() const;

    /**
     * @brief Get the initial condition tensor.
     * @return The initial condition as a DynamicTensor.
     */
    DynamicTensor GetInitialCondition() const;

    /**
     * @brief Get the dimension of the ODE system.
     * @return The dimension as a vector as dimension depends on the tensor.
     */
    std::vector<int> GetInputDimension() const;
    
    /**
     * @brief Get the step size for numerical solving.
     * @return The step size as a double.
     */
     double GetStepSize() const;
    
     /**
     * @brief Get the number of steps for numerical solving.
     * @return The number of steps as an integer.
     */
    int GetNumberOfSteps() const;

    /**
     * @brief Get the tolerance for numerical methods.
     * @return The tolerance as a double.
     */
    double GetTolerance() const;

    /**
     * @brief Get the maximum number of iterations for iterative methods.
     * @return The maximum number of iterations as an integer.
     */
    int GetMaxIterations() const;

    /**
     * @brief Get the solver method to be used.
     * @return The solver method as a SolverMethod enum.
     * This is only the text representation; it does not return the actual solver object.
     */
    SolverMethod GetSolverMethod() const;

    /**
     * @brief Get the RHS function defining the ODE.
     * @return A function pointer or object representing the RHS function.
     * This is a placeholder; actual implementation depends on how functions are represented.
     */
    // FunctionType GetRHSFunction() const;

    /**
     * @brief Set the RHS function defining the ODE.
     * @param function A function pointer or object representing the RHS function.
     * This is a placeholder; actual implementation depends on how functions are represented.
     */
    // void SetRHSFunction(FunctionType function);

    /**
     * @brief Set the solver method to be used.
     * @param method The solver method as a SolverMethod enum.
     */
    void SetSolverMethod(SolverMethod method);

    /**
     * @brief Set the maximum number of iterations for iterative methods.
     * @param max_iterations The maximum number of iterations as an integer.
     */
    void SetMaxIterations(int max_iterations);

    /**
     * @brief Set the tolerance for numerical methods.
     * @param tolerance The tolerance as a double.
     */
    void SetTolerance(double tolerance);

    /**
     * @brief Set the step size for numerical solving.
     * @param step_size The step size as a double.
     */
    void SetStepSize(double step_size);

    /**
     * @brief Set the number of steps for numerical solving.
     * @param number_of_steps The number of steps as an integer.
     */
    void SetNumberOfSteps(int number_of_steps);

    /**
     * @brief Set the initial condition tensor.
     * @param y0 The initial condition as a DynamicTensor.
     */
    void SetInitialCondition(const DynamicTensor& y0);

    /**
     * @brief Set the initial time.
     * @param t_init The initial time as a double.
     */
    void SetInitialTime(double t_init);

    /**
     * @brief Set the final time.
     * @param t_final The final time as a double.
     */
    void SetFinalTime(double t_final);

    /**
     * @brief Evaluate the RHS function at a given time and state.
     * @param t The current time.
     * @param y The current state as a DynamicTensor.
     * @return The result of the RHS function as a DynamicTensor.
     */
    DynamicTensor EvaluateRhs(double t, const DynamicTensor& y) const;

private:
    /// @brief Initial time
    double t_init_;
    /// @brief Final time
    double t_final_;
    /// @brief Initial condition tensor
    DynamicTensor y0_;
    /// @brief Dimension of the ODE system (size and shape depend on tensor)
    std::vector<int> input_dim_;
    /// @brief Step size for numerical solving
    double step_size_;
    /// @brief Number of steps for numerical solving
    int number_of_steps_;
    /// @brief Tolerance for numerical methods
    double tolerance_;
    /// @brief Maximum number of iterations for iterative methods
    int max_iterations_;
    
    /// @brief Function defining the ODE's right-hand side
    //function pointer or contains actual function class we create?

    /// @brief Solver method to be used. This is just an enum, not the actual solver. It is also changeable and does not restrict the solver used.
    SolverMethod solver_method_enum_;

    /**
     * @brief Private method to parse the initial condition from raw string
     * @param y0_string The string representing the initial condition, coming from raw data.
     * This method sets the y0_ DynamicTensor based on the provided string.
     */
    void ParseInitialConditionFromString(const std::string& y0_string);

    /**
     * @brief Private method to parse the RHS function from raw string
     * @param function_string The string representing the RHS function, coming from raw data.
     * This method sets the function pointer or object based on the provided string.
     */
    void ParseRhsFunctionFromString(const std::string& function_string);

    /**
     * @brief Private method to parse the solver method from raw string
     * @param method_string The string representing the solver method, coming from raw data.
     * This method sets the solver_method_enum_ based on the provided string.
     */
    void ParseSolverMethodFromString(const std::string& method_string);

    /**
     * @brief Private method to verify consistency of provided conditions.
     * @param raw_data The raw data used to initialize the ODE.
     * This method checks for compatibility between final time, step size, and number of steps, used during initialization.
     */
    void VerifyConditions(const OdeRawData& raw_data);

    void SetDimensionFromInitialCondition();
    /// @brief Fallback values for various parameters if not provided in raw data
    static constexpr double kDefaultStepSize = 0.01;
    static constexpr int kDefaultNumberOfSteps = 100;
    static constexpr double kDefaultTolerance = 1e-6;
    static constexpr int kDefaultMaxIterations = 1000;
    static constexpr double kDefaultInitialTime = 0.0;
    static constexpr double kDefaultFinalTime = 1.0;

};


#endif // ODE_H_