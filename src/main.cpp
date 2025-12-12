#include <iostream>
#include <cmath>
#include <memory>

<<<<<<< HEAD
=======
#include "output_plotter.h" 
#include "adamsbashforth_solver.h"
#include "ode.h"
#include "function.h"
#include "dynamic_tensor.h"
#include "reader_txt.h"
>>>>>>> refs/remotes/origin/main



int main() {
<<<<<<< HEAD

=======
    try{
        std::cout << "--- Reading from data/input.txt ---" << std::endl;
        TxtReader reader("data/input.txt", ';', '#');
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to read input file. " << e.what() << std::endl;
        return 1;
    }
    try{
        std::cout << "--- Constructing the ODE system from the reader ---" << std::endl;
        TxtReader reader("data/input.txt", ';', '#');
        Ode ode(reader);
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to construct ODE from reader. " << e.what() << std::endl;
        return 1;
    }
    
>>>>>>> refs/remotes/origin/main
    return 0;
}