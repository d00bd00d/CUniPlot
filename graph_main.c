#include "graph.h"
#include "plot.h"
#include "plot_options.h" 

#include <argp.h> 
#include <stdio.h>
#include <stdlib.h>

// all non-printable argument keys need to be in the range 3##. 
// I may separate this into its own graph_options file later, but
// right now there's not a lot of customisability to offer. 
#define INTERPOLATION_KEY   300             // not implemented yet 
#define EQUATION_KEY        ARGP_KEY_ARG

struct argp_option graph_params[] = {
    {"interpolant", INTERPOLATION_KEY, "LINEAR | SPLINE | STEP", 0,
        "The interpolant to use for user-supplied data (not "
        "implemented yet)."}, 
    {0}
}; 

// these are the options that we will need to keep track of
typedef struct graph_options {
    enum interpolant i; 
    char* equation; 
    plot_options* plot_opts; 
} graph_options; 

// argument parser! assumes that state->input is a pointer to a 
// graph_options struct. 
error_t parse_graph_params(int key, char* arg, 
                           struct argp_state* state) {
    graph_options* opts = state->input; 

    // pass the plot_options to the child parser 
    state->child_inputs[0] = opts->plot_opts; 

    switch(key) {
           case INTERPOLATION_KEY: // not implemented yet
    break; case EQUATION_KEY: 
        opts->equation = arg; 
    }

    return 0;
}

int main(int argc, char** argv) {
    // initialise the graph options 
    plot_options plot_opts = default_plot_options(); 
    graph_options opts = {
        LINEAR,         // interpolant
        NULL,           // equation to plot 
        &plot_opts
    }; 

    struct argp_child argp_children[] = { 
        {&plot_options_argp, 0, "General Plot Options: ", 1}, 
        {0} 
    }; 
    struct argp argp = {graph_params, parse_graph_params, 
        "[EXPRESSION]", 
        "Create a continuous line plot of some interpolated data "
        "or some function in x using unicode characters. If no "
        "input expression is provided, it defaults to the data file; "
        "if no data file is provided, it reads daat from stdin.", 
        argp_children
    }; 

    argp_parse(&argp, argc, argv, 0, 0, &opts); 

    // creating the plot - determine the content based on if the
    // user has provided an equation to use or not. 
    const char** content; 
    if(opts.equation == NULL) 
        content = data_to_graph(opts.i, opts.plot_opts); 
    else
        content = expression_to_graph(opts.equation, opts.plot_opts);
    
    draw_plot(content, opts.plot_opts); 
    free(content); 
    return 0; 
} 
