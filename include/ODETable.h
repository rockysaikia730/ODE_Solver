//
// Created by andra on 21/11/2025.
//

#ifndef CODES_ODETABLE_H
#define CODES_ODETABLE_H

#include <vector>
#include "ODE.h"
#include "TableEntry.h"

template <typename  T>
class ODETable: public ODE<T> {
public:
    using State = std::vector<T>;
    //ODETable(const Reader & );
    virtual ~ODETable() = default;
    ODETable(const std::vector<TableEntry<T>>& entries, const std::string& name, double tol = 1e-6);
    ODETable(const TableEntry<T>& entries, const std::string& name, double tol = 1e-6);
    virtual State evaluate(double t, const State& y) const override;
    virtual State evaluate(double t, const T& y) const override;
    virtual State evaluate(int i) const;
    void setTable(const std::vector<TableEntry<T>>& entries) {table = entries;}
    const std::vector<TableEntry<T>>& getTable() const {return table;}

private:
    std::vector<TableEntry<T>> table;
    double tol = 1e-6;
};

#endif //CODES_ODETABLE_H