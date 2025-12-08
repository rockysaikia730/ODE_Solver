#include <memory>
#include "ode_raw_data.h"


OdeRawData::OdeRawData(const OdeRawData& copy) {
    
    y0 = copy.y0;
    time_params = copy.time_params;
    solver_params = copy.solver_params;
    
    if (copy.function) {
        function = copy.function->Clone();
    } 
    else {
        function = nullptr;
    }
}

OdeRawData& OdeRawData::operator=(const OdeRawData& other) {
    if (this != &other) {
        
        y0 = other.y0;
        time_params = other.time_params;
        solver_params = other.solver_params;
        
        if (other.function) {
            function = other.function->Clone();
        } 
        else {
            function = nullptr;
        }
    }
    return *this;
}

