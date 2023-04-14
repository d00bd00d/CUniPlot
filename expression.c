#include "expression.h" 
#include "list.h"

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// no tokens over 14 chars, pretty reasonable
#define MAX_TOKEN_LENGTH 15 

/** 
 *  Creates a new expression node using malloc and returns a pointer
 *  to it. All pointers are initialised to the null pointers. 
 */ 
expression* create_node() {
    expression* e = (expression*) malloc(sizeof(expression)); 
    e->left = NULL; 
    e->right = NULL; 
    e->value = 0; 
    return e; 
} 

/** 
 *  Deletes an entire expression tree rooted at the provided node.
 */ 
void delete_tree(expression* tree) {
    if(tree == NULL) return; 
    delete_tree(tree->left); 
    delete_tree(tree->right); 
    free(tree); 
} 

/** 
 *  Evaluate the expression at a provided value of x. 
 */ 
double evaluate(expression* e, double x) {
    expression* l = e->left; 
    expression* r = e->right; 

    switch(e->operation) {
        // constant value 
        case CONSTANT:  return e->value; 
        case VARIABLE:  return x; 

        // arithmetic operations 
        case ADD:       return evaluate(l, x) + evaluate(r, x); 
        case SUBTRACT:  return evaluate(l, x) - evaluate(r, x); 
        case MULTIPLY:  return evaluate(l, x) * evaluate(r, x); 
        case DIVIDE:    return evaluate(l, x) / evaluate(r, x); 
        case POWER:     return pow(evaluate(l, x), evaluate(r, x)); 

        // trigonometric functions 
        case SINE:      return sin(evaluate(l, x)); 
        case COSINE:    return cos(evaluate(l, x)); 
        case TANGENT:   return tan(evaluate(l, x)); 
        case SECANT:    return 1.0 / cos(evaluate(l, x)); 
        case COSECANT:  return 1.0 / sin(evaluate(l, x)); 
        case COTANGENT: return 1.0 / tan(evaluate(l, x)); 

        // inverse trig functions 
        case ARCSIN:    return asin(evaluate(l, x)); 
        case ARCCOS:    return acos(evaluate(l, x)); 
        case ARCTAN:    return atan(evaluate(l, x)); 
        
        // log and exp 
        case LOG:       return log(evaluate(l, x)); // natural log 
        case EXP:       return exp(evaluate(l, x)); 
    } 

    return 0;
} 

// helper struct to tokenise a string input. Each token is a 
// syntactic unit of an expression. For instance, the expression 
// sin(3 * x + 4) + x ^ -5 yields 12 tokens: 
// sin, (, 3, *, x, +, 4, ), +, x, ^, -5. 
// these tokens can be classified. The helper function converts 
// a raw input string into a list of tokens that can be parsed. 
enum token_type {
    OPERATOR,           // +-*/^, the binary operators 
    OPEN_PAREN,         // ( and ), not much to say here
    CLOSE_PAREN,    
    NUMBER,             // things like 15, -4.5, etc. 
    VAR_TOKEN,          // the variable x. 
    STRING              // anything else, primarily functions like sin
};  

typedef struct token {
    enum token_type type; 
    char contents[MAX_TOKEN_LENGTH]; 
} token; 

// helper function to create a new token. rather than copying the
// entire string, it copies the first n characters in the string. 
token* make_token(enum token_type type, const char* s, int n) {
    token* t = (token*) malloc(sizeof(token)); 
    t->type = type; 
    for(int i = 0; i < n; i++) t->contents[i] = s[i]; 
    t->contents[n] = 0; 
    return t; 
} 

// helper function to determine if a character could be part of a 
// numerical token or not. 
bool in_number(char c) {
    return c == '-' || c == '.' || ('0' <= c && c <= '9'); 
}

// helper function to determine if a character is part of a string
// token or not, i.e. it's not an operator, parenthesis, or number.
bool in_string(char c) {
    return (strchr("+-*/^()", c) == NULL) && 
           !in_number(c) && !isspace(c); 
}

// helper function to extract a numeric token from the beginning of 
// the string. this advances the string to the last character within 
// the token. 
token* extract_number(const char** s) {
    int n = 1; 
    while(in_number((*s)[n]) && (*s)[n] != '-') { ++n; } 
    enum token_type type = NUMBER; 
    token* t = make_token(type, *s, n); 
    *s += n - 1; 
    return t; 
} 

// helper function to extract a string token from the beginning of 
// the string. this also advances the string to the last char within
// the token. 
token* extract_string(const char** s) {
    int n = 1; 
    while(in_string((*s)[n])) { ++n; } 

    // special case: the string is x, which is a variable token 
    enum token_type type; 
    if(**s == 'x' && n == 1) type = VAR_TOKEN; 
    else                     type = STRING; 

    token* t = make_token(type, *s, n); 
    *s += n -1; 
    return t;
} 


// creates a new list and populates it with the tokens extracted
// from the list. when possible, whitespace is ignored. whitespace is
// necessary to delemit two adjacent string tokens of similar type,
// so "sinx" and "sin x" would be different, as would "4 5" and "45".
// However, "4+5" and "4 + 5" are equivalent. this does not check for
// the syntactic correctness of the provided string expression. 
list* tokenise(const char* s) {
    list* tokens = create_list(); 
    enum token_type t; 

    while(*s) {
        // skip whitespace 
        if(isspace(*s)) { ++s; continue; }

        // handle the tokens consisting of a single character 
        if(strchr("+*/^", *s) != NULL) {
            t = OPERATOR; 
            append_list(tokens, make_token(t, s, 1)); 
        } else if(*s == '(') {
            t = OPEN_PAREN; 
            append_list(tokens, make_token(t, s, 1)); 
        } else if(*s == ')') {
            t = CLOSE_PAREN; 
            append_list(tokens, make_token(t, s, 1)); 
        }

        // a '-' character may be an operator or a number. check the
        // next character, which must be numeric and not another '-'.
        else if(*s == '-') {
            if(!in_number(*(s + 1)) || *(s + 1) == '-') {
                t = OPERATOR; 
                append_list(tokens, make_token(t, s, 1)); 
            } else append_list(tokens, extract_number(&s)); 
        } 

        // otherwise, we have a string, variable, or number token. 
        else if(in_number(*s)) 
            append_list(tokens, extract_number(&s)); 
        else              
            append_list(tokens, extract_string(&s)); 

        ++s; 
    } 

    return tokens; 
} 

// helper function to convert a leaf token (i.e. a constant or a 
// variable token) into a node in the expression tree. 
expression* make_leaf(token* t) {
    expression* e = create_node(); 
    if(t->type == VAR_TOKEN) {
        e->operation = VARIABLE; 
    } else if(t->type == NUMBER) {
        e->operation = CONSTANT; 
        e->value = strtod(t->contents, NULL); 
    } 

    // if it's not either of those two types, then this is an 
    // impossible situation. 
    else {
        free(e);
        return NULL; 
    }

    return e; 
} 

// helper construct to better convert between the contents of a
// function token to an operator enum type.
#define NUM_FUNCS 11 

static const struct op_pair {
    const char* contents;
    enum operators op; 
} STRING_TO_FUNC[NUM_FUNCS] = {
    {"sin", SINE}, {"cos", COSINE}, {"tan", TANGENT}, 
    {"sec", SECANT}, {"csc", COSECANT}, {"cot", COTANGENT}, 
    {"asin", ARCSIN}, {"acos", ARCCOS}, {"atan", ARCTAN}, 
    {"log", LOG}, {"exp", EXP} 
}; 

// helper function to convert an interior token (i.e. an operator or
// a function call) into an expression node, but without setting 
// the children. 
expression* make_node(token* t) {
    enum operators op; 
    switch(t->type) {
    case OPERATOR: 
        switch(t->contents[0]) {
               case '+': op = ADD; 
        break; case '-': op = SUBTRACT; 
        break; case '*': op = MULTIPLY; 
        break; case '/': op = DIVIDE; 
        break; case '^': op = POWER; 
        }
    break; case STRING: 
        int i = 0; 
        for(; i < NUM_FUNCS; i++) {
            if(strcmp(t->contents, STRING_TO_FUNC[i].contents) == 0) {
                op = STRING_TO_FUNC[i].op; 
                break; 
            }
        }

        if(i == NUM_FUNCS) return NULL; // didn't match the function
    break; default: return NULL; // improper branching point. 
    }

    expression* e = create_node(); 
    e->operation = op; 
    return e; 
} 

// helper function to convert a list of tokens into a full-on 
// expression tree. it will only convert the tokens between indices
// start (inclusive) and end (exclusive). returns a null pointer 
// if it could not parse the tokens. 
expression* parse_tokens(list* tokens, size_t start, size_t end); 

// helper function to make the recursive call to parse_tokens, 
// provided an index to split the token list at. 
expression* split_tokens(list* tokens, size_t start, size_t end, 
                         int branch) {
    // if the branch is -1, then there were no splitting points.
    // the only way this is valid is if there's a pair of paren-
    // theses around the entire expression. 
    if(branch < 0) {
        if(((token*)(tokens->data[start]))->type != OPEN_PAREN || 
           ((token*)(tokens->data[end - 1]))->type != CLOSE_PAREN) 
            return NULL; 

        return parse_tokens(tokens, start + 1, end - 1); 
    }

    // otherwise, make the interior node, parse the one or two sides,
    // and put them together before returning. 
    token* branch_token = (token*) tokens->data[branch]; 
    expression* middle = make_node(branch_token); 
    if(!middle) return NULL;    // error with branch token

    // if it's an operator, parse two sides 
    if(branch_token->type == OPERATOR) {
        expression* l = parse_tokens(tokens, start, branch);
        expression* r = parse_tokens(tokens, branch + 1, end); 

        // deal with parsing error
        if(!l || !r) { 
            delete_tree(middle); 
            delete_tree(l); 
            delete_tree(r); 
            return NULL; 
        }

        middle->left = l; 
        middle->right = r; 
        return middle; 
    } 

    // if it's a function, make sure it's the first token in the
    // list, then parse everything that's left. this is the only 
    // way a function is the branch point! 
    if(branch != start) return NULL; 
    expression* arg = parse_tokens(tokens, start + 1, end); 
    if(!arg) { delete_tree(middle); return NULL; } 
    middle->left = arg; 
    return middle; 
} 

// implementation of parse_tokens
expression* parse_tokens(list* tokens, size_t start, size_t end) {
    // if there's only 1 token, it has to be a leaf. 
    if(end == start + 1) 
        return make_leaf((token*)(tokens->data[start])); 

    int num_parens = 0; 
    int add_sub = -1, mul_div = -1, pow = -1, func = -1; 
    for(size_t i = start; i < end; i++) {
        token* t = (token*)(tokens->data[i]); 
        enum token_type type = t->type; 

        if(type == OPEN_PAREN) ++num_parens; 
        if(type == CLOSE_PAREN) --num_parens; 
        if(num_parens < 0) return NULL; 
        if(num_parens > 0) continue; 

        switch(type) {
        case VAR_TOKEN: case NUMBER: continue; 
        case STRING: if(func < 0) func = i; break; 

        // found an operator -- check if it's +- or */ or ^
        case OPERATOR: 
            switch(t->contents[0]) {
            case '+': case '-':        if(add_sub < 0) add_sub = i; 
            break; case '*': case '/': if(mul_div < 0) mul_div = i; 
            break; case '^':           if(pow < 0)     pow = i; 
            }
        break; default: 
        } 
    }

    // mismatched parentheses - bad syntax
    if(num_parens != 0) return NULL; 
    
    // reverse order of operations to make the branches 
    int branch = -1; 
    if(add_sub >= 0)        branch = add_sub; 
    else if(mul_div >= 0)   branch = mul_div; 
    else if(pow >= 0)       branch = pow; 
    else if(func >= 0)      branch = func;
    return split_tokens(tokens, start, end, branch); 
} 

/** 
 *  Constructs an expression tree out of a string input and returns
 *  a pointer to the root node. If the string input cannot be parsed,
 *  a null pointer is returned instead. 
 */ 
expression* parse_expression(const char* s) {
    // tokenise, then pass to recursive helper 
    list* tokens = tokenise(s); 
    expression* e = parse_tokens(tokens, 0, tokens->size); 
    delete_list(tokens); 
    return e;
} 
