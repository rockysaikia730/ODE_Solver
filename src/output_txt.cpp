#include <fstream>
#include "output.h"
#include "ode_solver.h"
#include "output_txt.h"


//---------------------------------------------------------------------//
// Constructor(s)
//---------------------------------------------------------------------//
OutputTxt::OutputTxt(const std::string& filename, char separator, char comment_char, char element_separator)
    : Output(filename, separator, element_separator), comment_char_(comment_char) {
        EnsureFileExists();
    }

//---------------------------------------------------------------------//
// Getters and Setters
//---------------------------------------------------------------------//

// set the comment character
void OutputTxt::SetCommentChar(char comment_char) {
    comment_char_ = comment_char;
}

// get the comment character
char OutputTxt::GetCommentChar() const {
    return comment_char_;
}

// get the header lines
std::vector<std::string> OutputTxt::GetHeader() const {
    return header_;
}

// set the header lines
void OutputTxt::SetHeader(const std::vector<std::string>& header) {
    header_ = header;
}

//---------------------------------------------------------------------//
// Protected Methods
//---------------------------------------------------------------------//

// File specific setup for TXT output
void OutputTxt::FileSpecificSetup(std::ofstream& file) const {
    // Write comment lines
    for (const auto& line : header_) {
        file << comment_char_ << " " << line << "\n";
    }
}