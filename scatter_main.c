#include "plot.h"
#include "plot_options.h"
#include "scatter.h"

#include <argp.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    // there are only the plot's options to worry about 
    plot_options plot_opts = default_plot_options(); 
    argp_parse(&plot_options_argp, argc, argv, 0, 0, &plot_opts); 

    // Create the plot here 
    const char** contents = data_to_scatter(&plot_opts); 
    draw_plot(contents, &plot_opts); 

    free(contents); 

    return 0; 
}
