//suggested by ChatGPT to handle Windows compatibility for popen/pclose
#ifdef _WIN32
#define popen  _popen
#define pclose _pclose
#endif

#ifndef OUTPUT_PLOTTER_H_
#define OUTPUT_PLOTTER_H_

#include <cstdio>       // For FILE*
#include <vector>       // For std::vector
#include <utility>      // For std::pair
#include <fstream>      // For std::ofstream (required for FileSpecificSetup signature)
#include "output.h"


class OutputPlotter : public Output {
public:
    /**
     * @brief Constructor
     */
    OutputPlotter();

    /**
     * @brief Destructor. Closes the pipe to Gnuplot.
     */
    ~OutputPlotter();

    /**
     * @brief Sends the current solver state to Gnuplot.
     * @param solver The solver instance containing the current time and solution.
     */
    void Write(const OdeSolver& solver) override;

private:
    FILE* gnuplot_pipe_;
    mutable std::vector<std::pair<double, double>> history_;
};

#endif