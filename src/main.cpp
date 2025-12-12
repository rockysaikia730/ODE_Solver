#include <iostream>
#include <cmath>
#include <memory>

#include "output_plotter.h" 

#include "ode.h"
#include "function.h"
#include "dynamic_tensor.h"

#include "reader_txt.h"

#include "adamsbashforth_solver.h"
#include "adam_moulton_solver.h"
#include "backward_differentiation_solver.h"
#include "backward_euler_solver.h"
#include "forward_euler_light_solver.h"
#include "runge_kutta_solver.h"



int main() {

    bool done = true;
    // while(done) {
    //     try{
    //         std::cout << "--- Reading from data/input.txt ---" << std::endl;
    //         TxtReader reader("data/input.txt", ';', '#');
    //     }
    //     catch (const std::exception& e) {
    //         std::cerr << "Failed to read input file. " << e.what() << std::endl;
    //         return 1;
    //     }
    //     try{
    //         std::cout << "--- Constructing the ODE system from the reader ---" << std::endl;
    //         TxtReader reader("data/input.txt", ';', '#');
    //         Ode ode(reader);
    //     }
    //     catch (const std::exception& e) {
    //         std::cerr << "Failed to construct ODE from reader. " << e.what() << std::endl;
    //         return 1;
    //     }

    //     try {
    //         AdamsBashforth adam_bashforth(ode);

    //         AdamMoulton adam_moulton(ode);

    //         Bdf bdf4(ode);
            
    //         RungeKutta rk4(ode);

    //         BackwardEuler be(ode);

    //         ForwardEulerLight fe(ode);
    //     }
    //     catch (const std::invalid_argument &e) {
    //         std::cout << e.what() << std::endl;
    //         std::cout << "Press a character once you modify your input : " << std::endl;
    //         std::string s;
    //         std::cin >> s;
    //         continue;
    //     }
    //     catch (const std::runtime_error &e) {
    //         std::cerr << e.what() << std::endl;
    //     }
    //     catch {...} {
    //         std::cout << "Some problem occurred" << std::endl;
    //     }
    // }
    return 0;
}