/** 
 *  Implementation file for histogram.h. 
 */ 

#include "histogram.h" 
#include "hist_options.h"
#include "list.h"

#include <stdbool.h>
#include <stdlib.h>

// important constants for drawing full-width and half-width bars. 
#define FULL_WIDTH_RES 9 
const char* FULL_WIDTH_BARS[FULL_WIDTH_RES] = {
    " ", "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█"
}; 

#define HALF_WIDTH_RES 4
const char* HALF_WIDTH_BARS[HALF_WIDTH_RES][HALF_WIDTH_RES] = {
    {" ", "🬞", "🬦", "▐"}, 
    {"🬏", "🬭", "🬵", "🬷"}, 
    {"🬓", "🬱", "🬹", "🬻"}, 
    {"▌", "🬲", "🬺", "█"}
}; 

// helper functions and structs, which are implemented later. 
int compare_data(const void*, const void*); 
list* read_data(FILE*); 
void rescale_plot(list*, hist_options*, plot_options*); 
double* get_freqs(list*, bool, plot_options*); 
const char** make_full_content(double*, plot_options*); 
const char** make_half_content(double*, plot_options*); 

/** 
 *  Reads data from the input file provided in the plot options, then
 *  constructs a histogram out of the data. 
 */ 
const char** data_to_histogram(hist_options* hist_opts, 
                               plot_options* plot_opts) {
    // read the raw data and rescale data accordingly. 
    list* data = read_data(plot_opts->data_input); 
    rescale_plot(data, hist_opts, plot_opts); 

    // create the bars in plot coordinates, then create the contents.
    double* bars = get_freqs(data, hist_opts->relative, plot_opts); 

    // populate the contents, free memory, then return. 
    const char** contents; 
    if(hist_opts->full_width)
        contents = make_full_content(bars, plot_opts); 
    else 
        contents = make_half_content(bars, plot_opts); 
    
    free(bars); 
    delete_list(data); 
    return contents; 
}

/** Implementations of helper functions. **/ 
// reads in a list of data points from the provided input file. 
list* read_data(FILE* fp) {
    list* data = create_list(); 
    double* temp = malloc(sizeof(double)); 
    
    while(fscanf(fp, " %lf", temp) != EOF) {
        append_list(data, temp); 
        temp = malloc(sizeof(double)); 
    }

    free(temp); 
    return data; 
}

// rescales a plot according to the features of the list of data 
// points to plot. 
void rescale_plot(list* data, hist_options* hist_opts, 
                  plot_options* plot_opts) {
    // don't do anything if the plot isn't to be rescaled 
    if(!plot_opts->rescale) return; 

    // otherwise, set the x axis scale to the minimum and maximum
    // data points, if a rescaling is needed. 
    double temp; 
    for(int i = 0; i < data->size; i++) {
        temp = *(double*)(data->data[i]); 
        if(temp < plot_opts->x_min) plot_opts->x_min = temp; 
        if(temp > plot_opts->x_max) plot_opts->x_max = temp; 
    }

    // the y-min MUST be zero, under any and all circumstances. 
    plot_opts->y_min = 0; 

    // rescale y-max to be the maximum absolute/relative frequency.
    if(hist_opts->relative) plot_opts->y_max = 1; 
    else                    plot_opts->y_max = data->size; 
}

// retrieves an array of absolute or relative frequencies for each
// bin, where the bins are determined based on the plot options.
// two bins are created per plot column. 
double* get_freqs(list* data, bool relative, 
                  plot_options* plot_opts) {
    int num_bins = plot_opts->width * 2; 
    double* bins = calloc(num_bins, sizeof(double)); 

    double bin_width = (plot_opts->x_max - plot_opts->x_min); 
    bin_width /= num_bins; 

    for(int i = 0; i < data->size; i++) {
        double x = *(double*)(data->data[i]); 
        int bin = (x - plot_opts->x_min) / bin_width; 

        // if we're at exactly the maximum, put that in the
        // right-most bin. 
        if(x == plot_opts->x_max) bin -= 1; 

        if(bin >= num_bins || bin < 0) continue; 
        bins[bin] += 1; 
    }

    // scale down for relative frequencies. 
    if(relative) 
        for(int i = 0; i < num_bins; i++) 
            bins[i] /= data->size; 

    return bins; 
}

// creates a histogram out of the frequency data using full-width
// bars. 
const char** make_full_content(double* bins, 
                               plot_options* plot_opts) {
    int plot_size = plot_opts->width * plot_opts->height; 
    const char** contents = malloc(plot_size * sizeof(char*)); 

    double y_per_subchar = (plot_opts->y_max - plot_opts->y_min); 
    y_per_subchar /= plot_opts->height * (FULL_WIDTH_RES - 1);
    int min_height = plot_opts->height * (FULL_WIDTH_RES - 1); 

    for(int row = 0; row < plot_opts->height; row++) {
        min_height -= FULL_WIDTH_RES - 1; 

        for(int col = 0; col < plot_opts->width; col++) {
            // merge the two neighbouring half-width bins for the 
            // full-width column before computing height
            double y = bins[2 * col] + bins[2 * col + 1]; 
            y -= plot_opts->y_min; 
            int height = y / y_per_subchar - min_height; 
            if(height < 0) height = 0; 
            if(height >= FULL_WIDTH_RES) height = FULL_WIDTH_RES - 1;

            int index = row * plot_opts->width + col; 
            contents[index] = FULL_WIDTH_BARS[height]; 
        }
    }

    return contents; 
}

const char** make_half_content(double* bins, 
                               plot_options* plot_opts) {
    int plot_size = plot_opts->width * plot_opts->height; 
    const char** contents = malloc(plot_size * sizeof(char*)); 

    double y_per_subchar = (plot_opts->y_max - plot_opts->y_min); 
    y_per_subchar /= plot_opts->height * (HALF_WIDTH_RES - 1);
    int min_height = plot_opts->height * (HALF_WIDTH_RES - 1); 

    for(int row = 0; row < plot_opts->height; row++) {
        min_height -= HALF_WIDTH_RES - 1; 

        for(int col = 0; col < plot_opts->width; col++) {
            // compute heights separately for the left and right 
            // subplots. 
            double left_y = bins[2 * col] - plot_opts->y_min; 
            int l = left_y / y_per_subchar - min_height;
            if(l < 0) l = 0; 
            if(l >= HALF_WIDTH_RES) l = HALF_WIDTH_RES - 1; 
            
            double right_y = bins[2 * col + 1] - plot_opts->y_min; 
            int r = right_y / y_per_subchar - min_height;
            if(r < 0) r = 0; 
            if(r >= HALF_WIDTH_RES) r = HALF_WIDTH_RES - 1; 

            int index = row * plot_opts->width + col; 
            contents[index] = HALF_WIDTH_BARS[l][r]; 
        }
    }

    return contents; 
   
}
