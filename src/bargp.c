#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


void count_args(size_t* total_args, size_t* n_opt_args, const struct ArgumentDefinition* argdefs)
{
    size_t i = 0;
    *total_args = 0;
    *n_opt_args = 0;


    while(argdefs[i].type != 0)
    {
        *total_args += 1;
        if (argdefs[i].is_optional)
        {
            *n_opt_args += 1;
        }
        i += 1;
    }
}


void* get_arg(const struct VTable* vtable, const struct ArgumentDefinition* argdef)
{
    const size_t tablei = get_hash(vtable, argdef);


    return vtable->table[tablei].value;
}


size_t get_hash(const struct VTable* vtable, const struct ArgumentDefinition* argdef)
{
    const size_t name_len = strlen(argdef->name);
    size_t nametol = 0;


    for (size_t i = 0; i < name_len; i += 1)
    {
        nametol += argdef->name[i];
    }

    return (nametol + argdef->key) % vtable->size;
}


int parse_args(
        struct VTable* vtable,
        const int argc,
        const char** argv,
        const struct ArgumentDefinition* argdefs
) {
    size_t tablei;


    return BARGP_SUCCESS;
}


void vtable_create(struct VTable* vtable)
{
    vtable->size = BARGP_MAX_NAME_LEN * BARGP_N_CHARS_ALPHA;
    vtable->table = (struct ArgDefToValue*)malloc(sizeof(struct ArgDefToValue*) * vtable->size);
}


void vtable_destroy(struct VTable* vtable)
{
    for (size_t i = 0; i < vtable->size; i += 1)
    {
        free(vtable->table[i].value);
    }
    free(vtable->table);
}


#endif  /* BARGP_C */
