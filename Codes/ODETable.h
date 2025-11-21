//
// Created by andra on 21/11/2025.
//

#ifndef CODES_ODETABLE_H
#define CODES_ODETABLE_H

#include <vector>
#include "ODE.h"

template <typename  T>
class ODETable: public ODE<T> {
public:
    using State = std::vector<T>;
    // Struct to hold a single table entry
    struct TableEntry {
        double t;
        State y;
        State f;
    };
    //ODETable(const Reader & );
    virtual ~ODETable() = default;
    ODETable(const std::vector<TableEntry>& entries, const std::string& name, double tol = 1e-6);
    ODETable(const TableEntry& entries, const std::string& name, double tol = 1e-6);

    virtual State evaluate(double t, const State& y) const override;
    virtual State evaluate(double t, const T& y) const override;
    virtual State evaluate(int i) const;
    void setTable(const std::vector<TableEntry>& entries) {table = entries;}
    const std::vector<TableEntry>& getTable() const {return table;}

private:
    std::vector<TableEntry> table;
    double tol = 1e-6;
};

#endif //CODES_ODETABLE_H