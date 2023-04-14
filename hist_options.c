/** 
 *  Implementation file for hist_options.h
 */ 

#include "hist_options.h"

#include <argp.h>
#include <stdbool.h>

// all non-printable argp keys need to be in the range 4##. 
#define RELATIVE_KEY    400
#define FULL_WIDTH_KEY  401 

static struct argp_option hist_params[] = {
    {"relative", RELATIVE_KEY, 0, 0, "Creates a plot of relative "
        "frequencies rather than absolute frequencies."}, 
    {"full-width", FULL_WIDTH_KEY, 0, 0, "Uses full-width bars "
        "instead of half-width bars for the plot."}, 
    { 0 } 
}; 

/** 
 *  The argp struct passed to the argp parser.
 */ 
struct argp hist_options_argp = {
    hist_params, parse_hist_params, 0, 0
}; 

/** 
 *  The argument parser for the histogram's options. It assumes
 *  that state->input points to a hist_options struct. 
 */ 
error_t parse_hist_params(int key, char* arg, 
                          struct argp_state* state) {
    hist_options* opts = state->input; 

    switch(key) {
           case RELATIVE_KEY: 
        opts->relative = true; 
    break; case FULL_WIDTH_KEY: 
        opts->full_width = true; 
    }

    return 0;
}

/** 
 *  Creates and returns a set of default options for the histogram.
 */ 
hist_options default_hist_options() {
    hist_options opts = { 
        .relative = false, 
        .full_width = false
    }; 

    return opts; 
}
