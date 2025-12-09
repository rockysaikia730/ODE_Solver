#ifndef TENSOR_PARSER_H
#define TENSOR_PARSER_H

#include <vector>
#include <string>
#include <stdexcept>
#include "parsed_tensor.h"

class TensorParser {
public:
    ParsedTensor ParseTensor(const std::string& str);
private:
    ParsedTensor ParseTensorRecursive(const std::string& str, size_t& pos, std::vector<size_t>& shape);

    void SkipWhitespace(const std::string& str, size_t& pos) const;

    std::string ParseNumberString(const std::string& str, size_t& pos) const;
};