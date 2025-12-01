
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "reader_csv.h"
#include "../Data_structures/ode_raw_data.h"

FileReader::FileReader(const std::string& file_name, char separator)
    : Reader(file_name), separator_(separator) {}


bool FileReader::Open() {
    file_stream_.open(file_name_);
    return file_stream_.is_open();
}

void FileReader::Close() {
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

void FileReader::SetSeparator(char separator) {
    separator_ = separator;
}

char FileReader::GetSeparator() const {
    return separator_;
}

std::vector<std::string> FileReader::Split(const std::string& str) {
    std::vector<std::string> tokens; // to store the split tokens
    std::string token;
    std::istringstream token_stream(str); // Reading from the input string
    while (std::getline(token_stream, token, separator_)) {
        tokens.push_back(token); // add each token to the vector
    }
    return tokens;
}

std::vector<std::string> FileReader::Trim(const std::vector<std::string>& tokens) {
    std::vector<std::string> trimmed_tokens; // to store the trimmed tokens

    for (const auto& token : tokens) {
        size_t start = 0; // index of the first non-space character
        size_t end = token.length(); // index one past the last non-space character

        // First non-space character from the beginning
        while (start < end && std::isspace(token[start])) {
            ++start;
        }

        // Find the last non-space character
        while (end > start && std::isspace(token[end - 1])) {
            --end;
        }

        // Extract the trimmed token
        trimmed_tokens.push_back(token.substr(start, end - start));
    }
    return trimmed_tokens;
}
