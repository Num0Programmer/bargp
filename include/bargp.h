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
#define BARGP_TOO_FEW_ARGUMENTS 60
#define BARGP_TOO_MANY_ARGUMENTS 70

/* hash table construction */
#define BARGP_MAX_NAME_LEN 64
#define BARGP_N_CHARS_ALPHA 27  // every letter [a-z][A-Z] and '-'

/* parsing list of values */
#define BARGP_LIST_DELIM ","


enum ArgType {
    LONG = 1,
    DOUBLE,
    STRING,
};


struct ArgumentDefinition {
    char key;
    char* name;
    bool is_list;
    bool is_optional;
    enum ArgType type;
};


struct ArgDefToValue
{
    const struct ArgumentDefinition* argdef;
    void* value;
};


struct VTable {
    size_t n_opt_keys;
    size_t n_opt_names;
    size_t n_stats;
    struct ArgDefToValue** keystoargs;   // optional arguments by key
    struct ArgDefToValue* namestoargs;   // optional arguments by name
    struct ArgDefToValue* stats;         // static 'positional' arguments
};


void count_args(
    size_t* total_args,
    size_t* n_opt_args,
    const struct ArgumentDefinition* argdefs,
    const char** argv,
    const int argc
);


void* get_arg_index(const struct VTable* vtable, const size_t index);


void* get_arg_key(const struct VTable* vtable, const char key);


void* get_arg_name(const struct VTable* vtable, const char* name);


size_t get_hash_key(const struct VTable* vtable, const char key);


size_t get_hash_name(const struct VTable* vtable, const char* argdef);


void help_fmt(
    const struct VTable* vtable,
    const struct ArgumentDefinition* argdefs,
    const size_t total_args
);


int parse_args(
    struct VTable* vtable,
    const int argc,
    const char** argv,
    const struct ArgumentDefinition* argdefs
); 


void vtable_create(
    struct VTable* vtable,
    const size_t total_args,
    const size_t n_opt_args,
    const struct ArgumentDefinition* argdefs
);


void vtable_destroy(struct VTable* vtable);


#endif  /* BARGP_H */
