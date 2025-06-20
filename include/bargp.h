#ifndef BARGP_H
#define BARGP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


/* Codes for identifying parse failure */
#define BARGP_SUCCESS 0
// general error
#define BARGP_FAILURE 1
#define BARGP_TOO_FEW_ARGUMENTS 60;
#define BARGP_TOO_MANY_ARGUMENTS 70;


enum ArgType {
    LONG = 1,
    DOUBLE,
    STRING,
    NARG_TYPES,  // tracks the number of argument types - should always remain at the end
};


struct ArgumentDefinition {
    char* name;
    enum ArgType type;
};


struct VTable {
    size_t size;
    void* values;
};


int parse_args(
    void* args,
    const int argc,
    const char** argv,
    const struct ArgumentDefinition* argdefs
);


#endif  /* BARGP_H */
