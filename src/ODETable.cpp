//
// Created by andra on 21/11/2025.
//

#include "ODETable.h"
#include <vector>
#include "ODE.h"
#include <stdexcept>
#include <iostream>
#include "VectorOutput.h"
using namespace std;

template <typename  T>
ODETable<T>::ODETable(const vector<TableEntry>& entries, const string& name, double tol): ODE<T>(
    entries.empty() ? 0.0 : entries[0].t,
    entries.empty() ? typename ODE<T>::State{} : entries[0].y,
    name),
    table(entries),
    tol(tol)
{}

template <typename  T>
ODETable<T>::ODETable(const TableEntry& entries, const string& name, double tol): ODE<T>(
    entries.empty() ? 0.0 : entries[0].t,
    entries.empty() ? typename ODE<T>::State{} : entries[0].y,
    name),
    table(entries),
    tol(tol)
{}

template <typename T>
ODETable<T>::State ODETable<T>::evaluate(double t, const State& y) const
{
    bool tfound = false;
    for (const auto& entry : table) {
        if (abs(entry.t - t) <= tol) {
            tfound = true;
            if (entry.y.size() != y.size()) {
                throw std::runtime_error("t found, but y has wrong size at this entry");
            }
        }
        bool yfound = false;
        for (size_t i = 0; i < y.size(); ++i) {
            if (std::abs(entry.y[i] - y[i]) >= tol) {
                yfound = false;
                break;
            }
        }
        if (yfound) {
            return entry.f;  // Success: both t and y match
        } else {
            throw std::runtime_error("t found, but y does not match at this entry");
        }
    }
    if (!tfound) {
        throw std::runtime_error("No matching t found in table");
    }
    return State{};
}

template <typename T>
ODETable<T>::State ODETable<T>::evaluate(double t, const T& y) const{
    for (auto& entry : table) {
        if (abs(entry.t - t) <= tol) {
            if (abs(entry.y[0] - y) >= tol) {
                throw std::runtime_error("t found, but y has wrong at this entry");
            }
            return entry.f;
        }
    }
    throw std::runtime_error("Not found in table");
}

template <typename T>
ODETable<T>::State ODETable<T>::evaluate(int i) const {
    if (i < 0 || i >= table.size()) {
        throw std::out_of_range("Index i out of range: valid 0 <= i < table.size()");
    }
    return table[i].f;
}

int main() {

    cout << "===========================================" << endl;
    cout << "Test: ODETable" << endl;

    // 1D table entries
    ODETable<double>::TableEntry e1{0.0, {1.0}, {2.0}};
    ODETable<double>::TableEntry e2{1.0, {2.0}, {4.0}};
    ODETable<double>::TableEntry e3{2.0, {3.0}, {6.0}};

    vector<ODETable<double>::TableEntry> tableEntries{e1, e2, e3};

    // Create ODETable
    ODETable<double> table1D(tableEntries, "TestTable1D");

    // Test evaluate by t and vector y
    try {
        auto res1 = table1D.evaluate(0.0, vector<double>{1.0});
        cout << "evaluate(t=0, y={1}) = " << res1[0] << endl;
    } catch (const exception& ex) {
        cout << "Error: " << ex.what() << endl;
    }

    // Test evaluate by t and scalar y (1D shortcut)
    try {
        auto res2 = table1D.evaluate(1.0, 2.0);
        cout << "evaluate(t=1, y=2) = " << res2[0] << endl;
    } catch (const exception& ex) {
        cout << "Error: " << ex.what() << endl;
    }

    // Test evaluate by index
    try {
        auto res3 = table1D.evaluate(2);
        cout << "evaluate(i=2) = " << res3[0] << endl;
    } catch (const exception& ex) {
        cout << "Error: " << ex.what() << endl;
    }

    // Test errors: wrong y
    try {
        auto res4 = table1D.evaluate(0.0, 0.5);
    } catch (const exception& ex) {
        cout << "Expected error: " << ex.what() << endl;
    }

    // Test errors: t not in table
    try {
        auto res5 = table1D.evaluate(5.0, 1.0);
    } catch (const exception& ex) {
        cout << "Expected error: " << ex.what() << endl;
    }

    // Test index out of bounds
    try {
        auto res6 = table1D.evaluate(10);
    } catch (const exception& ex) {
        cout << "Expected error: " << ex.what() << endl;
    }

    cout << "===========================================" << endl;
    cout << "All tests done!" << endl;

    return 0;
}