# ODE Solver Project Report

## 1. Implementation Overview

This project implements a general solver for non-linear ordinary differential equations of the form:
$$
\frac{d\mathbf{y}}{dt} = \mathbf{f}(\mathbf{y},t)
$$
More importantly, the project allows the reading of a problem from a `text` or `csv` file, then provides six different methods to solve the problem numerically:
1. Euler Backward method
2. Euler Forward method
3. Order 4 Adams Bashforth method
4. Order 4 Runge Kutta method
5. Adams Moulton method
6. Newton Raphson method

Below we will provide an overview of the classes and how they can be used.

### 1.1 Handling inputs

### 1.1.1 Reader Classes

At the time of writing, the `Reader` class is the parent virtual class for all readers in the project. It defines the shared interface for reading input and converting it into an `OdeRawData` object, which is then used to construct an `Ode`. The current implementations are `ReaderTxt` for text files and `ReaderCsv` for csv files. Notice that in our conception, the reader is not a dumb reader: it is tasked with creating tensor or function objects as well. The limitations will come at the end.

When reading from a file the format must follow a strict set of rules. Only key value pairs with recognised keys are processed. Each key expects a specific type of data, and incorrect or inconsistent data will cause the reader to throw an exception.

The accepted keys are listed below:

1. **`t`**  
   The initial time. Must be a real number. Parsed with `ParseDouble`.

2. **`tf`**  
   The final integration time. Must be a real number. Stored in `time_params.t_final`.

3. **`y`**  
   The initial condition. May be any tensor written in nested bracket form such as `[[1,2],[3,4]]`.  
   The parser deduces the shape and constructs a `DynamicTensor`.

4. **`step_size`**  
   The fixed step size for the solver. Must be a real number.

5. **`number_of_steps`**  
   An alternative to `step_size`. Specifies how many steps the solver should take. Must be an integer written as a real number.

6. **`solver_method`**  
   A string that specifies which solver method is used. The value is converted to lower case and passed to `ParseSolverMethodFromString`.

7. **`tolerance`**  
   A real number that defines the tolerance used by adaptive or iterative methods. Stored in `solver_params.tolerance`.

8. **`max_iterations`**  
   A whole number written as a real that specifies the maximum number of iterations allowed in methods that require internal iteration.

9. **`function`**  
   Defines the right hand side of the system.  
   The value must be a nested tensor of expressions.  
   The parser extracts:
   - `function_expressions`  
   - `function_shape`  
   No function object is created at this stage. This function creation is delegated to the sub-classes because the function needs to be created with the derivatives, and at the moment of parsing, we do not know if derivatives are provided or not.

10. **`derivative`**  
    Defines the Jacobian of the system if supplied. It must be a tensor of expressions of the same shape expected for the derivative of the vector field.  
    The parser extracts:
    - `derivative_expressions`  
    - `derivative_shape`  
    Providing a derivative is optional, but advised when wanting to use implicit methods.

Any other keys are ignored at this stage, although support for additional parameters can be added later.

The only distinction between `.txt` and `.csv` readers is how they handle formatting:

- **Text files (`ReaderTxt`)**  
  May contain comments that begin with `#`.  
  May store all key value pairs on one line.  
  If values are in one line then an `inline_separator` must be provided. The character `,` should not be used for this since it is reserved for tensor entries.

- **CSV files (`ReaderCsv`)**  
  May contain a header.  
  The header is skipped only if the user sets `header = true` in the constructor.

Both reader types eventually call `InterpretKeyValuePair` to process each key and store the resulting data in `OdeRawData`.

Finally, the `ReaderCsv` and `ReaderTxt` create a `ParsedFunction` instance.

### Shortcomings of the readers

1. **CSV files are not used in a conventional way**  
   A typical comma separated file stores numerical data arranged in columns. In this project a CSV file is treated as a list of key value pairs instead, which does not follow the usual structure. As a result the use of CSV files here is somewhat unnatural and limited.

2. **The reader requires a very strict input format**  
   Every key must appear exactly as expected and every value must follow the required structure. While this ensures correctness it also makes the input files less flexible and less user friendly.

3. **Parsing responsibilities are not separated cleanly**  
   The current reader performs several tasks that should ideally belong to a dedicated parser. In particular the logic for interpreting tensors and function expressions could be delegated to a `Parser` class. This would keep the reader simple and allow the parsing code to evolve independently. This design improvement was realised too late in the project and there was not enough time to restructure the code accordingly.

4. **There is no consistency check between the function and its derivative**  
   When both the right hand side function and its derivative are provided, no verification is made to ensure that:  
   - the derivative tensor has the correct shape,  
   - the derivative expressions match the expected dimension of the function,  
   - or that the expressions are mathematically compatible.  
   This must be improved to avoid silent errors when the derivative is supplied incorrectly.

### Running the Reader Unit Tests

The project uses CMake to configure and build all components, including the unit tests. The instructions below show how to configure the build directory and execute the tests.

#### On Windows (PowerShell or Command Prompt)

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Debug
ctest -C Debug
```

A test case running through a whole problem will be described in the end of the report.

---
### 1.1.2 The `OdeRawData` Structure

The `OdeRawData` structure acts as a container that stores all information extracted by the readers.  
It is not responsible for verification, consistency checks or object creation.  
Its purpose is simply to hold the raw user input in a structured format so that the `Ode` class can construct the actual mathematical object later.

An `OdeRawData` instance contains three groups of parameters:

1. **Time parameters**  
   These specify how the integration is to be carried out. They include:
   - `t0` the initial time  
   - `t_final` the final time  
   - `step_size` if fixed step integration is used  
   - `number_of_steps` as an alternative method to define the step size

2. **Solver parameters**  
   These parameters configure the behaviour of solvers that require iterative or adaptive strategies. They include:
   - `tolerance` a real number that controls accuracy  
   - `max_iterations` the maximum number of internal iterations  
   - `solver_method` an enumeration that describes which solver is used

3. **Function parameters**  
   These define the right hand side of the ordinary differential equation and optionally its derivative. They include:
   - `function_expressions` a flattened list of expressions  
   - `function_shape` the shape of the right hand side tensor  
   - `derivative_expressions` the Jacobian expressions if supplied  
   - `derivative_shape` the shape of the Jacobian  

Finally, `OdeRawData` contains the initial condition:

- `y0` a `DynamicTensor` that represents the initial state of the system

All fields in `OdeRawData` are written exactly once by the reader and are then consumed by the `Ode` constructor.  
The structure remains passive and does not attempt to interpret or validate the contents.

---
### 1.1.3 The `Ode` Class

The `Ode` class represents the full definition of an ordinary differential equation problem. It is constructed either directly from numerical values or indirectly from data supplied by a reader. Its main role is to gather all components required to describe the initial value problem and to provide uniform access to them for the solvers.

An instance of `Ode` always stores:

1. the initial time `t0`  
2. the initial condition `y0` as a `DynamicTensor`  
3. a name used to identify the system  
4. a pointer to a `Function` object that represents the right hand side  
5. an optional derivative function, supplied indirectly through the `Function` interface  
6. an optional `RootFinder` strategy for implicit solvers

The class offers several constructors in order to support different creation scenarios:

- constructing directly from numerical values and a `Function`  
- constructing from scalar initial conditions, which are promoted to a rank one tensor  
- constructing from a `Reader`  
- constructing from an `OdeRawData` structure  
- copying from another `Ode`

Once constructed, an `Ode` instance exposes the following responsibilities:

1. **Storing the initial conditions**  
   The initial time and initial state tensor are stored internally and accessed through `GetTimeIn` and `GetCondIn`.  
   These values remain unchanged throughout the lifetime of the solver unless explicitly modified through setter methods.

2. **Holding the right hand side function**  
   The pointer `func_` stores a `Function` instance that represents the map  
   $$
       f(t, y) = \frac{\mathrm{d}y}{\mathrm{d}t}.
   $$  
   The method `Evaluate` forwards the request directly to this function and returns its value as a `DynamicTensor`.

3. **Storing and providing access to the derivative**  
   The class supports analytical gradients of the right hand side through  
   $$
       \nabla f(t, y).
   $$  
   This is accessed through the method `Grad`. Internally, this is delegated to the derivative implementation provided by the `Function`.

4. **Providing a root finding strategy**  
   The optional `RootFinder` pointer is available for implicit solvers that must solve internal nonlinear systems. This is not provided by the reader and has remained unused for the time being. The `Ode` class does not perform any root finding itself; it simply stores the strategy for use by the solver.

5. **Providing safe access to its data**  
   The class contains getter methods for:
   - the initial time  
   - the initial condition  
   - the system name  
   - the right hand side function  
   - the root finder strategy  

   These methods ensure that solver classes can reliably access all required information without modifying it unintentionally.

The `Ode` class performs only limited validation.  
It assumes that the data provided by the reader or by the user is correct and consistent.  
It does not check the compatibility between the right hand side and the derivative function, nor does it verify the size of the tensors involved. Another shortcoming of the class is that we do not use the `RootFinder` for anyhting in the ode class, and the name of the `Ode` is also redundant for the time being.

### Running the Ode Unit Tests
The unittests can be run in the exact same way as the reader tests. In fact, all future tests are ran by those commands.

---
### 1.1.4 The `Function` Class

The `Function` class defines the abstract interface for representing the right hand side of an ordinary differential equation.  
It models a mapping of the form

$$
    f : (t, y) \longmapsto \frac{\mathrm{d}y}{\mathrm{d}t},
$$

where `y` is a `DynamicTensor` of arbitrary shape. Solvers interact only with this interface and remain unaware of the internal implementation of the function. A user can define any of their own functions that they want to test, simply by overloading the `Eval` method.

The class exposes two central operations:

1. **Evaluating the function**

   Every concrete subclass must implement  
   ```cpp
   virtual DynamicTensor Eval(double t, const DynamicTensor& y) const = 0;
   ```
   This method evaluates the function at time $t$ and at $y$. The output tensor must be compatible in shape with the state tensor used by the Ode, but this check is not made by the function.

2. **Computing the gradient**
   
   The class provides a default implementation of
   ```cpp
   DynamicTensor Grad(double t, const DynamicTensor& y, double dy = 0.1) const;
   ```
   which computes the gradient using a central finite difference scheme. This enables implicit solvers to approximate Jacobians even when no analytical derivative is supplied. Subclasses may override this method to provide an exact gradient.


The Function class is therefore an abstraction layer separating solver logic from mathematical evaluation. Solvers rely on this interface and remain entirely independent of how the function is implemented.

### Shortcomings of the Function class
Right now, there is no checking mechanism provided for veryfying the shape of the function and the input tensor. This responsibility is delegated to the user to verify for the time being.
---
### The ParsedFunction Class

The ParsedFunction class is a concrete implementation of the Function interface. It uses the `muParserX` external library to interpret mathematical expressions written as strings. This allows users to define ordinary differential equations entirely in input files without writing C++ code.

A `ParsedFunction` object is created from:
 - A list of expressions desribing the right hand side (one dimensional).
 - The shape of the output tensor (row-major).
 - An optional list of derivative and
 - their corresponding shape (if provided).

This structure supports both scalar and tensor valued ODE systems. The responsibilities of the class are as follows.

**Storing the expressions**
The right hand side expressions are stored in `expressions_` together with their shape in `function_shape_`. If analytical derivatives are supplied, they are stored in `derivatives_` and `derivative_shape_`. These strings remain unchanged after construction and are evaluated at runtime whenever the solver requests a function value.

**Evaluating the function**
parses and evaluates each expression using muParserX. The variables `t` and the components of the tensor `y` are injected into the parser. The method returns a DynamicTensor whose shape matches the one supplied at construction. Internally the evaluation is performed by the helper method Compute, which rebuilds the output tensor from the flattened list of computed values. The shape is verified for this method.

**Providing analytical gradients**

If derivative expressions are provided, the class overrides the gradient method so that
```cpp
DynamicTensor Grad(double t, const DynamicTensor& y) const
```
returns the exact analytical Jacobian. The derivative expressions are evaluated using the same mechanism as the main function. If no derivative expressions are supplied, the class falls back to the finite difference method inherited from the base class `Function`. The shape of the derivatives is not checked right now.

**Supporting tensor valued functions**

Because both the function and its derivative are represented as flat vectors of expressions combined with explicit shape information, the class naturally supports multidimensional output. The underlying evaluation logic does not need to change when the size of the system grows.

In practice, a `ParsedFunction` instance is usually created by a reader after parsing the `function` and `derivative` entries of an input file. This keeps the solvers entirely independent of muParserX and allows users to describe ODE systems symbolically in a flexible and readable format. 

---
### REMAINING TO DO THE SOLVER, Dynamic tensor, and the main.
# ODE_Solver

# For executing only the Doxyfile
```bash
cd docs
doxygen
```

# For compiling and generating the Doxygen documentation
```bash
mkdir build
cd build
cmake ..
make
```
