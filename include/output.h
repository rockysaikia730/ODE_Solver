#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <vector>
#include <string>
#include <fstream>
#include "dynamic_tensor.h"
#include "ode_solver.h"

class Output {
public:
    Output(const std::string& filename);
    virtual ~Output() = default;

    virtual void Write(double dt, const DynamicTensor& final_state) = 0;

protected:
    std::string filename_;
    std::ofstream output_file_;
};
#endif // OUTPUT_H_