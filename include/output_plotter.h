#ifndef OUTPUT_PLOTTER_H_
#define OUTPUT_PLOTTER_H_

#include <cstdio>       // For FILE*
#include <vector>       // For std::vector
#include <utility>      // For std::pair
#include <fstream>      // For std::ofstream (required for FileSpecificSetup signature)
#include "output.h"     // Base class


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
    void Write(const OdeSolver& solver) const override;

private:
    FILE* gnuplot_pipe_;
    mutable std::vector<std::pair<double, double>> history_;

    /**
     * @brief Override from Output base class.
     * Note: Since this class uses a pipe (FILE*) instead of an ofstream, 
     * this implementation is likely a placeholder.
     */
    void FileSpecificSetup(std::ofstream& file) const override;
};

#endif