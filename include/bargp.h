#ifndef BARGP_H
#define BARGP_H


enum ArgType {
    CHAR = 0,
    LONG,
    DOUBLE,
    STRING,
    NARG_TYPES  // tracks the number of argument types - should always remain at the end
};


struct ArgumentDefinition {
    char* name;
    enum ArgType type;
};


#endif  /* BARGP_H */
