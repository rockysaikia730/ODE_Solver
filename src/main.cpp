#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

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
#include "forward_euler_solver.h"

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
                reader = std::make_unique<TxtReader>(input_file, '=', '#', ';');
            } else if (input_ext == "csv") {
                reader = std::make_unique<CsvReader>(input_file, ';', false);
            } else {
                throw std::invalid_argument("Unsupported input file format: " + input_ext);
            }

            reader->Read();
            
            std::cout << "Constructing ODE problem from input file " << input_file << '\n';

            Ode ode(*reader);
            const auto& raw = reader->GetRawData();

            std::unique_ptr<OdeSolver> solver;
            std::shared_ptr<RootFinder> root_finder;
            root_finder = std::make_shared<NewtonRaphson>();

            switch (raw.solver_params.method) {
                case SolverMethod::kAdamBashforth:
                    solver = std::make_unique<AdamsBashforth>(ode, *reader);
                    break;

                case SolverMethod::kAdamMoulton:
                    solver = std::make_unique<AdamMoulton>(
                        ode, *reader, root_finder);
                    break;

                case SolverMethod::kBackwardDifferentiation:
                    solver = std::make_unique<Bdf>(
                        ode, *reader, root_finder);
                    break;

                case SolverMethod::kBackwardEuler:
                    solver = std::make_unique<BackwardEuler>(
                        ode, *reader, root_finder);
                    break;

                case SolverMethod::kForwardEulerLight:
                    solver = std::make_unique<ForwardEulerLight>(ode, *reader);
                    break;

                case SolverMethod::kRungeKutta:
                    solver = std::make_unique<RungeKutta>(ode, *reader);
                    break;
                
                case SolverMethod::kForwardEuler:
                    solver = std::make_unique<ForwardEuler>(ode, *reader);
                    break;

                default:
                    throw std::invalid_argument("Undefined solver method.");
            }

            std::cout << "Solving ODE...\n";

            if (std::string(argv[3]) == "1"){
                std::cout << "Plotting results...\n";
                OutputPlotter plotter;
                int iter = solver->GetNumberOfSteps();
                for(int i = 0; i < iter; ++i){
                    plotter.Write(*solver);  
                    solver->Step();
                }
                plotter.Write(*solver); 
            }
            else{
                solver->Solve();
            }

            //create output
            if (output_ext == "txt") {
                auto out = std::make_unique<OutputTxt>(output_file, '=', '#', ';');
                //out->SetHeader(std::vector<std::string>{"Final solution of the ODE"});
                output = std::move(out);
            } else if (output_ext == "csv") {
                auto out = std::make_unique<OutputCsv>(output_file, ';', true);
                //out->SetHeader(std::vector<std::string>{"Key; value"});
                output = std::move(out);
            } else {
                throw std::invalid_argument("Unsupported output file format: " + output_ext);
            }

            std::cout << "Writing final solution to " << output_file << "...\n";
            output->Write(*solver);

            std::cout << "Program finished successfully.\n";
            return 0;
        
        } catch (const std::invalid_argument &e) { 
            std::cerr << e.what() << std::endl; 
            std::cerr << "Press a character once you modify your input : " << std::endl; 
            std::string s; std::cin >> s; 
            continue; 
        } catch (const std::runtime_error &e) { 
            std::cerr << e.what() << std::endl; 
            std::cerr << "Program terminated." << std::endl;
            return 1;
        } catch (...) { 
            std::cout << "Some problem occurred" << std::endl; 
            return 1;
        }
    }
    return 0;
}
