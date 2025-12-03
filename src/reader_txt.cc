#include <fstream>
#include <string>
#include "ode_raw_data.h"
#include "reader_txt.h"

TxtReader::TxtReader(const std::string& file_name, char separator, char inline_separator, char comment_char)
    : Reader(file_name, separator), inline_separator_(inline_separator), comment_char_(comment_char) {}

void TxtReader::Read() {
    OdeRawData raw_data;
    bool is_open = Open();
    if (!is_open) {
        // Handle error: could not open file
    }

    // Strict format: lines starting with comment_char are comments
    std::string line;
    while (std::getline(file_stream_, line)) {
        // Skip comment lines
        if (line.empty() || line[0] == comment_char_) {
            continue;
        }

        // Parse key-value pairs separated by the separator character
        size_t sep_pos = line.find(separator_);
        if (sep_pos != std::string::npos) {
            std::string key = line.substr(0, sep_pos);
            std::string value = line.substr(sep_pos + 1);

            //Strict format: keys must be specific values. 
            // A single line may contain all values and keys, separated by inline_separator_
            // Example: t_init=0.0;t_final=10.0;step_size=0.01;number_of_steps=1000;y0=1.0;method=RungeKutta4;max_iterations=1000;tolerance=1e-6

            //Missing implementation
        }
    }
    Close();
}