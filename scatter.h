#ifndef SCATTER_H 
#define SCATTER_H 

#include "plot_options.h" 

// Creates a scatter plot out of the data in the plot's specified 
// data source. This assumes that the data are space-separated pairs
// of x and y coordinates. 
const char** data_to_scatter(plot_options*); 

#endif 
