/** 
 *  This header file encapsulates the command-line arguments that
 *  affect the plot's formatting, but not the plot's content. This
 *  includes things like the height and width of the plot, the scale
 *  of the plot, axis labels, etc., but it does not include options
 *  like the equation used for a line plot or if a histogram should
 *  use relative frequencies or not. 
 */ 

#ifndef PLOT_OPTIONS_H
#define PLOT_OPTIONS_H

#include <argp.h>
#include <stdbool.h>
#include <stdio.h>

// definition of the plot_options struct, which contains all of 
// the relevant user-defined data. 
typedef struct plot_options {
    // options for the geometry of the plot area 
    double x_min, x_max, y_min, y_max; 
    int width, height; 
    bool rescale; 

    // options for formatting the ticks and their precision
    int x_ticks, y_ticks; 
    int tick_precision; 

    // labels for each axis, as well as an option to control how
    // wide the y-axis labels should be. 
    char* x_label, * y_label, * title; 
    int y_label_width; 

    // option for the data input source. 
    FILE* data_input; 
} plot_options; 

// creates default options for the plot, which are arbitrarily
// defined by me
plot_options default_plot_options(); 

// the implementation of the argument parser itself 
error_t parse_plot_params(int, char*, struct argp_state*); 

// the argp struct needed to parse options for this section. 
extern struct argp plot_options_argp; 

#endif 
