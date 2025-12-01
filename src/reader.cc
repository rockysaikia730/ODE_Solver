#include "reader.h"
#include "../Data_structures/ode_raw_data.h"

Reader::Reader(const std::string& file_name)
    : file_name_(file_name) {
    // Extract file extension from file name
    size_t dot_position = file_name.find_last_of('.');
    if (dot_position != std::string::npos) {
        file_extension_ = file_name.substr(dot_position + 1);
    } else {
        file_extension_ = "";
    }
}

std::string Reader::GetFileName() const {
    return file_name_;
}

std::string Reader::GetFileExtension() const {
    return file_extension_;
}

void Reader::SetFileName(const std::string& file_name) {
    file_name_ = file_name;
}

void Reader::SetFileExtension(const std::string& file_extension) {
    file_extension_ = file_extension;
}

bool Reader::IsNumeric(const std::string& str) const {
    if (str.empty()) return false;

    char* endptr = nullptr;
    std::strtod(str.c_str(), &endptr); //convert string to double, if possible
    return (*endptr == '\0'); // if endptr points to the end of the string, it's all numeric
}