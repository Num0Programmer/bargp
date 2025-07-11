#ifndef BARGP_H
#define BARGP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Codes for identifying parse failure */
#define BARGP_SUCCESS 0
// general error codes
#define BARGP_FAILURE 1
#define BARGP_TOO_FEW_ARGUMENTS 60;
#define BARGP_TOO_MANY_ARGUMENTS 70;

/* Hash table construction */
#define BARGP_MAX_NAME_LEN 64
#define BARGP_N_CHARS_ALPHA 27  // every letter [a-z][A-Z] and '-'


enum ArgType {
    LONG = 1,
    DOUBLE,
    STRING,
    NARG_TYPES,  // tracks the number of argument types - should always remain at the end
};


struct ArgumentDefinition {
    char key;
    char* name;
    bool is_optional;
    enum ArgType type;
};


struct ArgDefToValue
{
    struct ArgumentDefinition* argdef;
    void* value;
};


struct VTable {
    size_t size;
    struct ArgDefToValue* table;
};


void count_args(size_t* total_args, size_t* n_opt_args, const struct ArgumentDefinition* argdefs);


void* get_arg(const struct VTable* vtable, const struct ArgumentDefinition* argdef);


size_t get_hash(const struct VTable* vtable, const struct ArgumentDefinition* argdef);


int parse_args(
    struct VTable* vtable,
    const int argc,
    const char** argv,
    const size_t total_args,
    const struct ArgumentDefinition* argdefs
);


void vtable_create(struct VTable* vtable);


void vtable_destroy(struct VTable* vtable);


#endif  /* BARGP_H */
