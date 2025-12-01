#include <fstream>
#include "reader_csv.h"
#include "../Data_structures/ode_raw_data.h"

CsvReader::CsvReader(const std::string& file_name, char separator)
    : FileReader(file_name, separator) {}


OdeRawData CsvReader::Read() {
    OdeRawData raw_data;
    bool is_open = Open();
    if (!is_open) {
        // Handle error: could not open file
        return raw_data;
    }

    std::string line;
    int line_number = 1;
    bool header_present = false;
    while (std::getline(file_stream_, line)) {
        std::vector<std::string> tokens = Split(line);
        
        // Skip header line if present
        if(line_number == 1){
            for(const auto& token : tokens){
                if(!IsNumeric(token)){
                    header_present = true;
                    break; // Exit the loop if a non-numeric token is found
                }
            }
        }
        
        line_number++;

        if(header_present && line_number == 1){
            line_number++;
            continue; // Skip header line
        }

        // Assume each line has key-value pairs for simplicity
        std::string key = tokens[0];
        std::string value = tokens[1];

        if(key == "t0"){
            raw_data.t_init_string = value;
        } 
        else if(key == "tfinal"){
            raw_data.t_final_string = value;
        } 
        else if(key == "step_size"){
            raw_data.step_size_string = value;
        } 
        else if(key == "number_of_steps"){
            raw_data.number_of_steps_string = value;
        } 
        else if(key == "max_iterations"){
            raw_data.max_iterations_string = value;
        } 
        else if(key == "tolerance"){
            raw_data.tolerance_string = value;
        } 
        else if(key == "input_dim"){
            raw_data.input_dim_string = value;
        } 
        else if(key == "y0"){
            raw_data.y0_string = value;
        } 
        else if(key == "function"){
            raw_data.rhs_function_string = value;
        }
    }

    file_stream_.close();

    return raw_data;
}