//
// Created by andra on 21/11/2025.
//

#ifndef CODES_VECTOROUTPUT_H
#define CODES_VECTOROUTPUT_H

#include <iostream>
#include <vector>
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i != vec.size() - 1) os << ", ";
    }
    os << "]";
    return os;
}
#endif //CODES_VECTOROUTPUT_H