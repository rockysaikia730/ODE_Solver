#include <string>
#include <cctype>
#include <fstream>
#include "output_csv.h"
#include "ode_solver.h"

//---------------------------------------------------------------------//
// Constructor(s)
//---------------------------------------------------------------------//
OutputCsv::OutputCsv(const std::string& filename, char separator, char element_separator)
    : Output(filename, separator, element_separator) {
        EnsureFileExists();
}

//---------------------------------------------------------------------//
// Getters and Setters
//---------------------------------------------------------------------//

// get the separator character
char OutputCsv::GetSeparator() const {
    return separator_;  
}

// set the separator character
void OutputCsv::SetSeparator(char separator) {
    separator_ = separator;
}

// get the header row
std::vector<std::string> OutputCsv::GetHeader() const {
    return header_;
}

// set the header row
void OutputCsv::SetHeader(const std::vector<std::string>& header) {
    header_ = header;
}

//---------------------------------------------------------------------//
// Protected Methods
//---------------------------------------------------------------------//

// File specific setup for CSV output
void OutputCsv::FileSpecificSetup(std::ofstream& file) const {
    // Write header row
    if(!header_.empty()) {
        for(size_t i = 0; i < header_.size(); ++i) {
            file << header_[i];
            if(i + 1 < header_.size()) {
                file << separator_;
            }
        }
        file << "\n";
    }
}


