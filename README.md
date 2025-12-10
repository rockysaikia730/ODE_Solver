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

### Reader Classes

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
---



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
