/** 
 *  Generic list or dynamic array data structure. Supports the usual 
 *  insertion, deletion, etc. functions; see below for a full list. 
 *  To support flexibility, the list stores void* pointers rather than
 *  actual values; the user is responsible for managing the typing. 
 */ 

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list {
    size_t capacity, size; 
    void** data; // stores an array of void*
} list; 

/** 
 *  Creates an empty list, with a default capacity of 8.
 */ 
list* create_list();   

/** 
 *  Destroys a list object allocated by create_list, and additionally
 *  frees any memory associated with the contents of the list. 
 */ 
void delete_list(list* l);   

/** 
 *  Doubles the capacity of a list without modifying its contents. 
 */ 
void expand_list(list* l); 

/** 
 *  Adds an item at the end of a list, doubling the capacity if the 
 *  item would not fit. 
 */ 
void append_list(list* l, void* item);   

/** 
 *  Inserts an item into a sorted list while maintaining the order of
 *  the provided list. A comparison function of type int(void*, void*)
 *  must be supplied in order to define an ordering of the elements. 
 */ 
void insert_sorted_list(list* l, void* item, 
                        int (*compare)(const void*, const void*)); 

#endif
