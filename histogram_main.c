#include "histogram.h"
#include "hist_options.h"
#include "plot.h"
#include "plot_options.h" 

#include <argp.h>
#include <stdio.h> 
#include <stdlib.h>

// helper struct that contains both the histogram options and the
// plot options. 
typedef struct all_options {
    hist_options* hist_opts; 
    plot_options* plot_opts; 
} all_options; 

// the main argp parser, which is mainly a wrapper that passes 
// things to the children parsers from hist_options and plot_options.
error_t parse_params(int key, char* arg, struct argp_state* state) {
    all_options* opts = state->input; 
    state->child_inputs[0] = opts->plot_opts; 
    state->child_inputs[1] = opts->hist_opts; 

    return 0; 
}

int main(int argc, char** argv) {
    // create default options for the histogram and plot 
    plot_options plot_opts = default_plot_options(); 
    hist_options hist_opts = default_hist_options(); 
    all_options opts = { &hist_opts, &plot_opts }; 

    // pass to the argp parser. 
    struct argp_child children[] = {
        {&plot_options_argp, 0, "General Plot Options: ", 1}, 
        {&hist_options_argp, 0, "Histogram Options: ", 2}, 
        { 0 }
    }; 

    struct argp argp = {
        0, parse_params, 0, 
        "Creates a histogram out of the data provided. ", 
        children
    }; 

    argp_parse(&argp, argc, argv, 0, 0, &opts); 

    // Now actually do something with the options. 
    const char** content = data_to_histogram(&hist_opts, &plot_opts);
    draw_plot(content, &plot_opts); 
    free(content); 

    return 0; 
}
