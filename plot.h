/** 
 *  This header provides all of the functions (actually, just one) 
 *  that are necessary for building the basic elements of the plot:
 *  the axes, tick marks, labels, and title. 
 */ 

#ifndef PLOT_H
#define PLOT_H 

#include "plot_options.h" 

/** 
 *  This function creates a plot, provided with a matrix of (unicode)
 *  strings representing the contents of the plot itself. This matrix
 *  should be formatted as a 1D array of char* (strings), where the
 *  string at location (col, row) corresponds to the string at index
 *  row * width + col. col and row are both indexed by zero and start
 *  at the upper-left-hand corner. 
 */ 
void draw_plot(const char** contents, plot_options* opts); 

#endif 
