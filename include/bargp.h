#ifndef BARGP_H
#define BARGP_H

#include <stdio.h>
#include <stdlib.h>


enum ArgumentParseError {
    ARG_NOT_PARSED = -255,
};


enum ArgType {
    LONG = 0,
    DOUBLE,
    STRING,
    NARG_TYPES,  // tracks the number of argument types - should always remain at the end
};


struct ArgumentDefinition {
    char* name;
    enum ArgType type;
};


enum ArgumentParseError parse_args(
    void* args,
    const int argc,
    const char** argv,
    const struct ArgumentDefinition* argdefs
);


#endif  /* BARGP_H */
