/** 
 *  Header file for the graphing utility, which produces a continuous
 *  (shaded) line plot of the provided equation or data. If the user
 *  provides data (in the form of space-separated x/y coordinate 
 *  pairs), then an interpolation function is used to determine the
 *  shape of the graph. Otherwise, the user may supply an expression
 *  that's a function of x to graph instead. 
 */ 

#ifndef GRAPH_H 
#define GRAPH_H

#include "plot_options.h" 

enum interpolant { // not implemented yet
    LINEAR, 
    SPLINE, 
    STEP
}; 

// reads data from the data input source specified from the plot 
// options, then determines the graph's contents from there. 
const char** data_to_graph(enum interpolant, plot_options*); 

// uses a string expression to create the plot's contents. 
const char** expression_to_graph(char*, plot_options*); 

#endif 
