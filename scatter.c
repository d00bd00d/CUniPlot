#include "list.h"
#include "plot_options.h"
#include "scatter.h" 

#include <stdio.h>
#include <stdlib.h>

// lists the 64 different possible characters that can be displayed.
// There are 6 bits in the index. The least significant bit is the
// upper-left sub-block, then the next bit is the upper-right, and 
// so on and so forth (left-to-right, up-to-down). 
const char* BLOCKS [64]= {" ", "🬀", "🬁", "🬂", "🬃", "🬄", "🬅", "🬆", 
                          "🬇", "🬈", "🬉", "🬊", "🬋", "🬌", "🬍", "🬎", 
                          "🬏", "🬐", "🬑", "🬒", "🬓", "▌", "🬔", "🬕", 
                          "🬖", "🬗", "🬘", "🬙", "🬚", "🬛", "🬜", "🬝", 
                          "🬞", "🬟", "🬠", "🬡", "🬢", "🬣", "🬤", "🬥", 
                          "🬦", "🬧", "▐", "🬨", "🬩", "🬪", "🬫", "🬬", 
                          "🬭", "🬮", "🬯", "🬰", "🬱", "🬲", "🬳", "🬴", 
                          "🬵", "🬶", "🬷", "🬸", "🬹", "🬺", "🬻", "█"}; 

// helper structs and functions 
typedef struct point { 
    double x, y; 
} point; 

list* read_data(FILE* fp); 
void rescale_bounds(list* data, plot_options* options); 

// Creates a scatter plot out of the data in the plot's specified 
// data source. This assumes that the data are space-separated pairs
// of x and y coordinates. 
const char** data_to_scatter(plot_options* options) {
    // read data and rescale plot bounds 
    list* data = read_data(options->data_input); 
    rescale_bounds(data, options); 

    // construct a list of block indices for each character on
    // the screen. 
    int plot_size = options->width * options->height; 
    int* indices = calloc(plot_size, sizeof(int)); 

    // width and height of each cell, for ease of computation 
    double w = (options->x_max - options->x_min) / options->width; 
    double h = (options->y_max - options->y_min) / options->height;

    for(int i = 0; i < data->size; i++) {
        point* p = data->data[i]; 

        // bounds checks - skip points outside the range of the plot
        if(p->x < options->x_min || p->x >= options->x_max || 
           p->y < options->y_min || p->y >= options->y_max) continue; 
        
        // get coordinates in the grid. Origin is bottom-left here
        int x = (p->x - options->x_min) / w; 
        int y = (p->y - options->y_min) / h; 

        // get subcharacter coordinates 
        int sub_x = (p->x - options->x_min) * 2.0 / w - 2 * x; 
        int sub_y = (p->y - options->y_min) * 3.0 / h - 3 * y; 

        // convert to top-right origin 
        sub_y = 2 - sub_y; 
        y = options->height - y - 1; 
        indices[y * options->width + x] |= 1 << (2 * sub_y + sub_x); 
    }

    // create the contents
    const char** contents = malloc(plot_size * sizeof(char*)); 
    for(int i = 0; i < plot_size; i++) {
        contents[i] = BLOCKS[indices[i]]; 
    }

    free(indices); 
    delete_list(data); 
    return contents; 
} 

// implementation of helper functions 

// reads a space-separated list of x y coordinate pairs into a list
// of data points. 
list* read_data(FILE* fp) {
    list* l = create_list(); 
    point* p = malloc(sizeof(point)); 
    while(fscanf(fp, " %lf %lf", &(p->x), &(p->y)) != EOF) {
        append_list(l, p); 
        p = malloc(sizeof(point)); 
    }

    free(p); 
    return l; 
}


// resets the scale of the plot to fit with the provided data, so
// long that the user has not forced their bounds. 
void rescale_bounds(list* data, plot_options* options) {
    // don't do anything if the user turned this off 
    if(!options->rescale) return; 

    // otherwise, uhh do thing 
    for(int i = 0; i < data->size; i++) {
        point p = *((point*)(data->data[i])); 
        if(p.x < options->x_min) options->x_min = p.x; 
        if(p.x > options->x_max) options->x_max = p.x; 
        if(p.y < options->y_min) options->y_min = p.y; 
        if(p.y > options->y_max) options->y_max = p.y; 
    }
} 
