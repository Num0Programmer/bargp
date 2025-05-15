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


void parse_args(
    void* args,
    const int argc,
    const char** argv,
    const struct ArgumentDefinition* argdefs
);


#endif  /* BARGP_H */
