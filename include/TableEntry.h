
#ifndef CODES_TableEntry_H
#define CODES_TableEntry_H

#include <vector>

template <typename T>
struct TableEntry {
    double t;
    std::vector<T> y;
    std::vector<T> f;
};

#endif //CODES_TableEntry_H