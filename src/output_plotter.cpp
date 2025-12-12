#include "output_plotter.h"
#include "ode_solver.h"      // Required to use solver.GetCurrentTime()
#include "dynamic_tensor.h"  // Required to use solver.GetSolution()
#include <stdexcept>
#include <iostream>
#include "output.h"

// Constructor
OutputPlotter::OutputPlotter() 
    : gnuplot_pipe_(nullptr), Output()
    {
    gnuplot_pipe_ = popen("gnuplot -persistent", "w");
    if (!gnuplot_pipe_) {
        throw std::runtime_error("Could not find or open Gnuplot. Please ensure Gnuplot is installed and in your system PATH.");
    }
    fprintf(gnuplot_pipe_, "set title 'Real-time ODE Solution'\n");
    fprintf(gnuplot_pipe_, "set xlabel 'Time (t)'\n");
    fprintf(gnuplot_pipe_, "set ylabel 'y(t)'\n");
    fprintf(gnuplot_pipe_, "set grid\n");
    fprintf(gnuplot_pipe_, "set term qt noraise\n");
}

OutputPlotter::~OutputPlotter() {
    if (gnuplot_pipe_) {
        pclose(gnuplot_pipe_);
        gnuplot_pipe_ = nullptr;
    }
}

void OutputPlotter::Write(const OdeSolver& solver) {
    double t = solver.GetCurrentTime();
    double y = solver.GetSolution().at<double>({0});
    history_.push_back({t, y});
    
    fprintf(gnuplot_pipe_, "plot '-' with lines linewidth 2 title 'Solution'\n");
    for (const auto& point : history_) {
        fprintf(gnuplot_pipe_, "%f %f\n", point.first, point.second);
    }
    fprintf(gnuplot_pipe_, "e\n");
    fflush(gnuplot_pipe_);
    
}