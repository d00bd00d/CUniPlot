/** 
 *  Implementation file for plot_options.h 
 */ 

#include "plot_options.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// All non-printable argument keys for plot options are of the form 
// 2##; other groups of options must not have keys of this form. 
#define X_TICK_KEY      256
#define Y_TICK_KEY      257
#define TICK_PREC_KEY   258
#define X_LABEL_KEY     259
#define Y_LABEL_KEY     260
#define PLOT_TITLE_KEY  261
#define Y_LABEL_W_KEY   262
#define DATA_INPUT_KEY  263 // not implemented yet 
#define NO_RESCALE_KEY  264

static struct argp_option plot_params[] = {
    {"x-min", 'x', "NUM", 0, "Lower bound for x-axis."},
    {"y-min", 'y', "NUM", 0, "Lower bound for y-axis."},
    {"x-max", 'X', "NUM", 0, "Upper bound for x-axis."},
    {"y-max", 'Y', "NUM", 0, "Upper bound for y-axis."},
    {"width", 'w', "NUM", 0, "Width of the plot in characters."},
    {"height", 'h', "NUM", 0, "Height of the plot in characters."},
    {"no-rescale", NO_RESCALE_KEY, 0, 0, "Prevents the graph from "
        "rescaling the axes to include all data points, which it "
        "does by default. Does nothing if the user is plotting an "
        "expression."}, 
    {"x-ticks", X_TICK_KEY, "NUM", 0, "Number of ticks on x-axis."},
    {"y-ticks", Y_TICK_KEY, "NUM", 0, "Number of ticks on y-axis."},
    {"tick-precision", TICK_PREC_KEY, "NUM", 0,
        "Precision used to display axis ticks."},
    {"x-label", X_LABEL_KEY, "LABEL", 0, "Label for the x-axis."},
    {"y-label", Y_LABEL_KEY, "LABEL", 0, "Label for the y-axis."},
    {"title", PLOT_TITLE_KEY, "TITLE", 0, "Title of the plot."}, 
    {"y-label-width", Y_LABEL_W_KEY, "NUM", 0,
        "Width of the y-axis label and ticks"},
    {0}
};

/** 
 *  The argp struct passed to the argp parser for this set of 
 *  arguments and options.
 */ 
struct argp plot_options_argp = {
    plot_params, parse_plot_params, 0, 0
}; 

/** 
 *  The argument parser for the plot's options. It assumes that 
 *  state->input is a pointer to a plot_options struct. 
 */ 
error_t parse_plot_params(int key, char* arg, 
                          struct argp_state* state) {
    plot_options* options = state->input; 
    errno = 0; 

    switch(key) {

    // PLOT GEOMETRY // 
    // setting limits of the graph 
           case 'x': 
        options->x_min = strtod(arg, NULL);       
    break; case 'X': 
        options->x_max = strtod(arg, NULL);       
    break; case 'y': 
        options->y_min = strtod(arg, NULL);       
    break; case 'Y': 
        options->y_max = strtod(arg, NULL);       

    // setting height and width of the graph 
    break; case 'w': 
        options->width = strtol(arg, NULL, 0); 
    break; case 'h': 
        options->height = strtol(arg, NULL, 0); 

    // automatic rescaling
    break; case NO_RESCALE_KEY: 
        options->rescale = false; 

    // AXIS TICK FORMAT // 
    // setting ticks on x and y axes 
    break; case X_TICK_KEY: 
        options->x_ticks = strtol(arg, NULL, 0); 
    break; case Y_TICK_KEY: 
        options->y_ticks = strtol(arg, NULL, 0); 

    // setting tick precision 
    break; case TICK_PREC_KEY: 
        options->tick_precision = strtol(arg, NULL, 0); 

    // LABELS AND LABEL FORMAT // 
    // axis labels and title
    break; case X_LABEL_KEY: 
        options->x_label = arg; 
    break; case Y_LABEL_KEY: 
        options->y_label = arg; 
    break; case PLOT_TITLE_KEY: 
        options->title = arg; 

    // width of the y labels (left of the plot) 
    break; case Y_LABEL_W_KEY: 
        options->y_label_width = strtol(arg, NULL, 0); 

    } // end of fat switch 

    return errno; 
}

/** 
 *  Returns a plot_options struct populated with default arguments.
 */ 
plot_options default_plot_options() {
    plot_options default_options = {
        // plot geometry
        .x_min = -2, 
        .x_max = 2, 
        .y_min = -2, 
        .y_max = 2, 
        .width = 60, 
        .height = 25, 
        .rescale = true, 

        // tick options and formatting 
        .x_ticks = 5, 
        .y_ticks = 5, 
        .tick_precision = 1, 

        // axis labels and formatting 
        .x_label = "x", 
        .y_label = "y", 
        .title = "", 
        .y_label_width = 10, 

        // data input 
        .data_input = stdin 
    }; 

    return default_options; 
}
