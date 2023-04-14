/** 
 *  Header file for the histogram utility. This program takes in 
 *  space-separated numerical data and constructs a histogram for it.
 *  When rescaling the axis bounds, the histogram will always force
 *  y_min = 0, unless the user supplies a --no-rescale option. In 
 *  addition, the histogram supports two different output styles:
 *  one with full-width bars (default) and one with half-width bars.
 *  The full-width bars have worse horizontal resolution but better
 *  vertical resolution; the half-width bars have the opposite 
 *  properties. 
 */ 

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "hist_options.h"
#include "plot_options.h"

// reads data from the provided input data source (in the plot 
// options), then constructs the content of the plot. The boolean
// argument specifies whether or not to use a full-width bar. 
const char** data_to_histogram(hist_options*, plot_options*); 

#endif 
