#ifndef BARGP_H
#define BARGP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    char key;
    char* name;
    enum ArgType type;
};


struct VTable {
    size_t size;
    void** values;
};


struct RecvArgs {
    size_t size;
    size_t* codes;
    char** values;
};


size_t count_args(const struct ArgumentDefinition* argdefs);


void* get_arg(const struct VTable* vtable, const struct ArgumentDefinition* argdef);


size_t get_hash(const struct VTable* vtable, const struct ArgumentDefinition* argdef);


int parse_args(
    struct VTable* vtable,
    const int argc,
    const char** argv,
    const struct ArgumentDefinition* argdefs
);


void vtable_create(struct VTable* vtable, const size_t size);


void vtable_destroy(struct VTable* vtable);


#endif  /* BARGP_H */
