/** 
 *  Implementation file for graph.h 
 */ 

#include "expression.h" 
#include "graph.h" 
#include "list.h"
#include "plot_options.h" 

// very important constants that help us retrieve the right unicode
// characters quickly. 
#define RESOLUTION 6
static const char* BLOCKS[RESOLUTION][RESOLUTION] = {
    {" ", " ", "🭇", "🭉", "🭋", "▐"}, 
    {" ", " ", "🭈", "🭊", "◢", "🭅"}, 
    {"🬼", "🬽", "🬭", "🭆", "🭄", "🭃"}, 
    {"🬾", "🬿", "🭑", "🬹", "🭂", "🭁"}, 
    {"🭀", "◣", "🭏", "🭍", "█", "█"}, 
    {"▌", "🭐", "🭎", "🭌", "█", "█"}
}; 

// Forward declarations of the helper structs and methods involved.
typedef struct point {
    double x, y; 
} point; 

int compare_points(const void*, const void*); 
const char* get_block(int, int, int, plot_options*); 
double linear_interp(list*, double); 
const char** points_to_contents(point*, plot_options*); 
list* read_points(FILE*); 
void rescale_bounds(list*, plot_options*); 
int y_to_height(double, plot_options*); 

/** 
 *  This function retrieves data from the data source indicated in 
 *  the plot options. Using that data, together with the specified
 *  interpolant, this then produces the necessary array of strings
 *  that represents the contents of the plot. 
 */ 
const char** data_to_graph(enum interpolant i, 
                           plot_options* plot_opts) {
    // first, obtain a list of data points from the input file
    list* data = read_points(plot_opts->data_input); 
    rescale_bounds(data, plot_opts); 

    // next, apply the interpolation function to create an array of
    // points corresponding to the "borders" between each column. 
    point* points = malloc((plot_opts->width + 1) * sizeof(point));
    double x = plot_opts->x_min; 
    double dx = (plot_opts->x_max - x) / plot_opts->width; 

    for(int i = 0; i <= plot_opts->width; i++) {
        points[i].x = x; 
        points[i].y = linear_interp(data, x); 
        x += dx; 
    }
        
    // convert to plot contents, then free memory and return. 
    const char** contents = points_to_contents(points, plot_opts); 
    free(points); 
    delete_list(data); // no longer needed
    return contents;    
}

/** 
 *  This function receives a string expression and a set of plotting
 *  options and uses it to compute what the plot should look like. 
 *  If the provided string expression is invalid, then the plot will
 *  be filled with empty space instead. 
 */ 
const char** expression_to_graph(char* equation, 
                                 plot_options* plot_opts) {
    expression* e = parse_expression(equation); 

    // error parsing expression 
    if(e == NULL) {
        int num_points = plot_opts->width * plot_opts->height; 
        const char** contents = malloc(num_points * sizeof(char*)); 
        for(int i = 0; i < num_points; i++) 
            contents[i] = " "; 
        return contents; 
    }

    // create a list of points again; this time, we know how many. 
    // use the equation to compute their values. 
    point* points = malloc((plot_opts->width + 1) * sizeof(point)); 
    double x = plot_opts->x_min; 
    double dx = (plot_opts->x_max - x) / plot_opts->width; 

    for(int i = 0; i <= plot_opts->width; i++) {
        points[i].x = x; 
        points[i].y = evaluate(e, x); 
        x += dx; 
    }

    const char** contents = points_to_contents(points, plot_opts); 
    delete_tree(e); 
    return contents; 
}

/** Implementations of helper functions **/ 
// compares two points, ordering first by x-coordinate and then
// by y-coordinate. 
int compare_points(const void* lhs, const void* rhs) {
    const point* p1 = lhs, * p2 = rhs; 

    // comparing x-coordinates to see if they're different 
    if(p1->x < p2->x)      return -1; 
    else if(p1->x > p2->x) return 1;

    // comparing y-coordinates now 
    if(p1->y < p2->y)      return -1; 
    else if(p1->y > p2->y) return 1;

    // both are equal
    return 0; 
}

// creates a list of points out of an input file pointer 
list* read_points(FILE* input) {
    list* data = create_list(); 

    point* temp = malloc(sizeof(point)); 
    while(fscanf(input, " %lf %lf", &(temp->x), &(temp->y)) != EOF) {
        insert_sorted_list(data, temp, &compare_points); 
        temp = malloc(sizeof(point)); 
    }

    free(temp); 
    return data; 
} 

// uses the provided list of data to linearly interpolate what 
// y should be at the specified value of x. 
double linear_interp(list* data, double x) {
    // if x is less than the first element of the list, set x to
    // the smallest x value and proceed per usual. 
    point* temp = data->data[0]; 
    if(x < temp->x) x = temp->x; 

    // now look for the point where x >= point.x, but x < the next
    // point's x-coordinate. 
    point* curr, * next; 
    for(int i = 0; i < data->size - 1; i++) {
        curr = data->data[i]; 
        next = data->data[i + 1]; 

        if(x < curr->x || x >= next->x) continue; 

        // perform the linear interpolation
        double slope = (next->y - curr->y) / (next->x - curr->x); 
        return (x - curr->x) * slope + curr->y; 
    }

    // return the largest y-value - x is too large for our data. 
    return ((point*)(data->data[data->size - 1]))->y; 
} 

// converts the provided value of y into the proper sub-pixel height
// (based on resolution) and returns it. 
int y_to_height(double y, plot_options* plot_opts) {
    double dy = plot_opts->y_max - plot_opts->y_min; 
    dy /= plot_opts->height; 

    return (y - plot_opts->y_min) * (RESOLUTION - 3) / dy; 
}

// Given the left and right heights for a provided block on a certain
// row, this function determines which character should represent
// that block. 
const char* get_block(int left, int right, int row, 
                      plot_options* plot_opts) {
    int min_height = plot_opts->height - row - 1; 
    min_height *= RESOLUTION - 3; 

    left = left - min_height + 1; 
    right = right - min_height + 1; 

    if(left < 0)            left = 0; 
    if(left >= RESOLUTION)  left = RESOLUTION - 1; 
    if(right < 0)           right = 0; 
    if(right >= RESOLUTION) right = RESOLUTION - 1; 
    
    return BLOCKS[left][right]; 
}

// receives an array of points that represent the function values
// in between each column of characters. It converts this data into
// the array of unicode characters that represent the plot area. 
const char** points_to_contents(point* points, 
                                plot_options* plot_opts) {
    int num_points = plot_opts->width * plot_opts->height; 
    const char** contents = malloc(num_points * sizeof(char*)); 

    for(int col = 0; col < plot_opts->width; col++) {
        for(int row = 0; row < plot_opts->height; row++) {
            int l = y_to_height(points[col].y, plot_opts); 
            int r = y_to_height(points[col + 1].y, plot_opts); 
            int index = row * plot_opts->width + col; 
            contents[index] = get_block(l, r, row, plot_opts); 
        }
    }

    return contents; 
}

// rescales the bounds in the provided plot options according to the
// data provided. does nothing if the rescaling feature is disabled.
void rescale_bounds(list* data, plot_options* plot_opts) {
    if(!plot_opts->rescale) return; 

    for(int i = 0; i < data->size; i++) {
        point* p = data->data[i]; 
        if(p->x < plot_opts->x_min) plot_opts->x_min = p->x; 
        if(p->x > plot_opts->x_max) plot_opts->x_max = p->x; 
        if(p->y < plot_opts->y_min) plot_opts->y_min = p->y; 
        if(p->y > plot_opts->y_max) plot_opts->y_max = p->y; 
    }
}
