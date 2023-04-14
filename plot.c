/** 
 *  Implementation file for plot.h. 
 */ 

#include "plot.h" 
#include "plot_options.h"

#include <stdio.h>
#include <stdlib.h>

/**
 *  This constant represents the unicode character for drawing the
 *  border of the plot. In the future, I may change this to be several
 *  characters for the horizontal/vertical sides and the four corners.
 */ 
#define BORDER "▒"

// forward declaration of several helper functions and structs. 
// these are not exposed in the header. 
#define F_STRING_LENGTH 100

typedef struct format_strings {
    char y_label_f[F_STRING_LENGTH];    
    char x_tick_f[F_STRING_LENGTH]; 
    char y_tick_f[F_STRING_LENGTH]; 

    // helpful for printing out empty parts of the y axis. 
    char y_empty_f[F_STRING_LENGTH]; 
} format_strings;

format_strings* make_format_strings(plot_options*); 
void make_main_plot(const char**, plot_options*, format_strings*); 
void make_x_axis(plot_options*, format_strings*); 

/** 
 *  Prints a full plot with the specified contents to stdout. 
 *  The plot's contents should be provided in a 1-dimensional array
 *  of char*'s, where the character at (col, row) should be at the
 *  index row * width + col. These coordinates are zero-indexed, 
 *  and the origin is in the top-left corner. 
 */ 
void draw_plot(const char** contents, plot_options* options) {
    format_strings* formats = make_format_strings(options); 
    make_main_plot(contents, options, formats); 
    make_x_axis(options, formats); 
    free(formats); 
} 

/** Implementations of helper functions and structs. **/ 

// using the plot options, this creates formatting strings that
// define how certain elements of the plot (i.e. axis labels and
// tick marks) are printed to the screen. 
format_strings* make_format_strings(plot_options* options) {
    format_strings* f = malloc(sizeof(format_strings)); 

    sprintf(f->y_label_f, "%%%ds ", options->y_label_width); 
    sprintf(f->y_empty_f, f->y_label_f, ""); 
    sprintf(f->y_tick_f, "%%%d.%dlf ", 
            options->y_label_width, 
            options->tick_precision); 
    sprintf(f->x_tick_f, "%%.%dlf ", options->tick_precision); 

    return f; 
}

// this draws the main plot area using the provided options and
// formatting strings. This includes the title, y axis labels and
// ticks, and the plot's contents itself. It does not make the
// x axis label or ticks. 
void make_main_plot(const char** contents, plot_options* options, 
                    format_strings* formats) {
    // print the title 
    printf("%s %s\n", formats->y_empty_f, options->title); 

    // print the y label and upper border 
    printf(formats->y_label_f, options->y_label); 
    for(int i = 0; i < options->width + 2; i++) printf(BORDER); 
    printf("\n"); 

    // print the main plot area. start by computing how many lines
    // correspond to each tick 
    int tick_rate = 0; 
    if(options->y_ticks > 1) 
        tick_rate = (options->height - 1) / (options->y_ticks - 1); 

    for(int row = 0; row < options->height; row++) {
        if(tick_rate > 0 && row % tick_rate > 0) 
            printf(formats->y_empty_f); 
        else {
            double y = row * (options->y_max - options->y_min); 
            y = options->y_max - y / options->height; 
            printf(formats->y_tick_f, y); 
        }

        printf(BORDER); 
        for(int col = 0; col < options->width; col++)
            printf("%s", contents[row * options->width + col]); 
        printf(BORDER); 
        printf("\n"); 
    }

    // print the bottom border
    printf(formats->y_empty_f); 
    for(int i = 0; i < options->width + 2; i++) printf(BORDER); 
    printf("\n"); 
} 

// This only draws the x axis ticks and the corresponding x label
// underneath it - nothing else. 
void make_x_axis(plot_options* options, format_strings* formats) {
    printf(formats->y_empty_f); 
    printf(" "); // extra space for left border

    // determine the tick rate. keep track of the number of ticks 
    // printed and the position along the x axis. 
    int num_ticks = 0, position = 0, tick_rate = 0; 
    if(options->x_ticks > 1) 
        tick_rate = (options->width - 1) / (options->x_ticks - 1); 

    // only print an x-tick if the number of ticks is less than the 
    // number of ticks we'd expect at the current position. Note
    // that we only need to print ticks at all if there are ticks
    // to print to begin with 
    if(tick_rate > 0) {
        while(position <= options->width) {
            if(num_ticks < position / tick_rate + 1) {
                double x = options->x_max - options->x_min; 
                x = options->x_min + x * position / options->width; 
                position += printf(formats->x_tick_f, x); 
                num_ticks++; 
            } else position += printf(" "); 
        }

        printf("\n"); 
    }

    // print the x label on the next line, directly beneath the
    // tick marks. 
    printf("%s %s\n", formats->y_empty_f, options->x_label); 
}
