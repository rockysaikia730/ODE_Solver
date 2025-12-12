#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "reader.h"
#include "reader_txt.h"
#include "reader_csv.h"

#include "ode.h"
#include "solver_methods.h"
#include "ode_solver.h"

#include "adamsbashforth_solver.h"
#include "adam_moulton_solver.h"
#include "backward_differentiation_solver.h"
#include "backward_euler_solver.h"
#include "forward_euler_light_solver.h"
#include "runge_kutta_solver.h"

#include "newton_raphson.h"
#include "output_plotter.h"

#include "output.h"
#include "output_txt.h"
#include "output_csv.h"

int main(int argc, char* argv[]){
    bool continue_loop = true;
    while(continue_loop){    
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0]
                    << " <input_file> <output_file> <plot_or_not_1_0>\n";
            return 1;
        }

        std::string input_file  = argv[1];
        std::string output_file = argv[2];

        std::unique_ptr<Reader> reader;
        std::unique_ptr<Output> output;

        // get extensions
        std::string input_ext  = input_file.substr(input_file.find_last_of('.') + 1);
        std::string output_ext = output_file.substr(output_file.find_last_of('.') + 1);

        // create reader
        try{
            if (input_ext == "txt") {
                reader = std::make_unique<TxtReader>(input_file, ';', '#');
            } else if (input_ext == "csv") {
                reader = std::make_unique<CsvReader>(input_file, ';', false);
            } else {
                throw std::invalid_argument("Unsupported input file format: " + input_ext);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error creating reader: " << e.what() << '\n';
            return 1;
        }

        try {
            std::cout << "Constructing ODE problem from input file " << input_file << '\n';

            Ode ode(*reader);
            const auto& raw = reader->GetRawData();

            std::unique_ptr<OdeSolver> solver;

            switch (raw.solver_params.method) {
                case SolverMethod::kAdamBashforth:
                    solver = std::make_unique<AdamsBashforth>(*reader, ode);
                    break;

                case SolverMethod::kAdamMoulton:
                    solver = std::make_unique<AdamMoulton>(
                        *reader, ode, std::make_unique<NewtonRaphson>());
                    break;

                case SolverMethod::kBackwardDifferentiation:
                    solver = std::make_unique<Bdf>(
                        *reader, ode, std::make_unique<NewtonRaphson>());
                    break;

                case SolverMethod::kBackwardEuler:
                    solver = std::make_unique<BackwardEuler>(
                        *reader, ode, std::make_unique<NewtonRaphson>());
                    break;

                case SolverMethod::kForwardEulerLight:
                    solver = std::make_unique<ForwardEulerLight>(*reader, ode);
                    break;

                case SolverMethod::kRungeKutta:
                    solver = std::make_unique<RungeKutta>(*reader, ode);
                    break;

                default:
                    throw std::invalid_argument("Undefined solver method.");
            }

            std::cout << "Solving ODE...\n";
            solver->Solve();

            if (std::string(argv[3]) == "1"){
                std::cout << "Plotting results...\n";
                OutputPlotter plotter;
                plotter.Write(*solver);
            }
            
            // create output
            if (output_ext == "txt") {
                auto out = std::make_unique<OutputTxt>(output_file, ' ', '#', ';');
                out->setHeader({"Final solution of the ODE"});
                output = std::move(out);
            } else if (output_ext == "csv") {
                auto out = std::make_unique<OutputCsv>(output_file, ';', true);
                out->SetHeader({"Key; value"});
                output = std::move(out);
            } else {
                throw std::invalid_argument("Unsupported output file format: " + output_ext);
            }

            std::cout << "Writing final solution to " << output_file << "...\n";
            output->Write(*solver);

            std::cout << "Program finished successfully.\n";
        } catch (const std::invalid_argument &e) { 
            std::cout << e.what() << std::endl; 
            std::cout << "Press a character once you modify your input : " << std::endl; 
            std::string s; std::cin >> s; 
            continue; 
        } catch (const std::runtime_error &e) { 
            std::cerr << e.what() << std::endl; 
        } catch (...) { 
            std::cout << "Some problem occurred" << std::endl; 
        }
    }
    return 0;
}
