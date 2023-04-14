/** 
 *  Implementation of the functions defined in list.h
 */ 
#include "list.h"

#include <stdlib.h> 

/** 
 *  Creates an empty list, with a default capacity of 8.
 */ 
list* create_list() {
    list* l = (list*) malloc(sizeof(list)); 

    l->capacity = 8; 
    l->size = 0; 
    l->data = (void**) malloc(8 * sizeof(void*)); 
    return l; 
} 

/** 
 *  Destroys a list object allocated by create_list. Though this 
 *  frees the memory associated with the list itself (i.e. the void**
 *  array), it does not actually delete the contents of that array. 
 */ 
void delete_list(list* l) {
    for(size_t i = 0; i < l->size; i++) 
        free(l->data[i]); 

    free(l->data); 
    free(l); 
}

/** 
 *  Doubles the capacity of a list without modifying its contents. 
 */ 
void expand_list(list* l) {
    l->capacity *= 2; 
    l->data = (void**)realloc(l->data, l->capacity * sizeof(void*)); 
} 

/** 
 *  Adds an item at the end of a list, doubling the capacity if the 
 *  item would not fit. 
 */ 
void append_list(list* l, void* item) {
    if(l->size >= l->capacity) expand_list(l); 
    l->data[ l->size ++ ] = item; 
}

/** 
 *  Inserts an item into a sorted list while maintaining the order of
 *  the provided list. A comparison function of type int(void*, void*)
 *  must be supplied in order to define an ordering of the elements. 
 */ 
void insert_sorted_list(list* l, void* item, 
                        int (*compare)(const void*, const void*)) {
    // expand the list if we need extra capacity 
    if(l->size >= l->capacity) expand_list(l); 

    // start at the back of the list. shift the end of the list 
    // back until you reach the right spot. 
    size_t index = l->size++; 
    while(index > 0) {
        int c = (*compare)(l->data[index - 1], item); 

        // if c > 0, then the current list item should be after
        // the new item. 
        if(c > 0) 
            l->data[index] = l->data[index - 1]; 

        // otherwise, the current item belongs here 
        else {
            l->data[index] = item; 
            return; 
        }

        index--; 
    } 

    // at this point, we've moved EVERYTHING up. thus the new
    // item belongs at the very front. 
    l->data[0] = item; 
} 
