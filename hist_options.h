/** 
 *  A header file for the various options that can be specified for
 *  a histogram plot. This includes things like using half vs. full-
 *  width precision, using relative frequencies, etc., but does not
 *  include options for the plot's axes or formatting (see 
 *  plot_options for those). 
 */ 

#ifndef HIST_OPTIONS_H
#define HIST_OPTIONS_H

#include <argp.h>
#include <stdbool.h>

// the hist_options struct, which contains the relevant information
// that can be used to construct a histogram plot. 
typedef struct hist_options {
    // using relative or absolute frequency 
    bool relative; 

    // using full-width or half-width unicode characters 
    bool full_width;
} hist_options; 

// creates a hist_options struct initialised with the default 
// settings.
hist_options default_hist_options(); 

// the actual argument parser to be used with argp 
error_t parse_hist_params(int, char*, struct argp_state*); 

// the argp struct needed for argp. 
extern struct argp hist_options_argp; 

#endif
