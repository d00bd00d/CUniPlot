#ifndef EXPRESSION_H
#define EXPRESSION_H 

/** 
 *  The struct representing a node in an expression tree. Leaf nodes
 *  have a set value and no children whereas operator nodes (both 
 *  binary operators like * and unary operators like sin) have one or
 *  two children but no set value. Apply the operator to the value of 
 *  the children by computing recursively down the tree!
 * 
 *  In addition, this supports the presence of exactly one named 
 *  variable x so that expressions like 3 * x + 4 are valid. 
 */ 
enum operators { 
    // for leaf nodes, which could be the variable x! 
    CONSTANT, VARIABLE, 

    // basic arithmetic 
    ADD, MULTIPLY, SUBTRACT, DIVIDE, POWER, 
    
    // trig functions 
    SINE, COSINE, TANGENT, SECANT, COSECANT, COTANGENT, 
    ARCSIN, ARCCOS, ARCTAN, 

    // log and exp 
    LOG, EXP
}; 

typedef struct expression {
    enum operators operation; 
    struct expression* left, * right; 
    double value;   
} expression; 

/** 
 *  Creates a new expression node using malloc and returns a pointer
 *  to it. Nothing fancy. 
 */ 
expression* create_node(); 

/** 
 *  Frees all of the memory associated with an expression tree rooted
 *  at the provided node. 
 */
void delete_tree(expression* tree); 

/** 
 *  Constructs an expression tree out of a string input and returns
 *  a pointer to the root node. If the string input cannot be parsed,
 *  a null pointer is returned instead. 
 */ 
expression* parse_expression(const char* s); 

/** 
 *  Evaluate the expression at a provided value of x. 
 */ 
double evaluate(expression* e, double x); 

#endif
