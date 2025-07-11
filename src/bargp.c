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


    return vtable->opts[tablei].value;
}


size_t get_hash(const struct VTable* vtable, const struct ArgumentDefinition* argdef)
{
    const size_t name_len = strlen(argdef->name);
    size_t nametol = 0;


    for (size_t i = 0; i < name_len; i += 1)
    {
        nametol += argdef->name[i];
    }

    return (nametol + argdef->key) % vtable->n_opts;
}


int parse_args(struct VTable* vtable, const int argc, const char** argv)
{
    size_t tablei;


    return BARGP_SUCCESS;
}


void vtable_create(
        struct VTable* vtable,
        const size_t total_args,
        const size_t n_opt_args,
        const struct ArgumentDefinition* argdefs
) {
    size_t tablei;


    vtable->n_stats = total_args - n_opt_args;
    vtable->n_opts = BARGP_MAX_NAME_LEN * BARGP_N_CHARS_ALPHA;
    vtable->opts = (struct ArgDefToValue*)malloc(sizeof(struct ArgDefToValue*) * vtable->n_opts);
    vtable->stats = (struct ArgDefToValue*)malloc(sizeof(struct ArgDefToValue*) * vtable->n_stats);
    for (size_t i = 0; i < total_args; i += 1)
    {
        if (argdefs[i].is_optional)
        {
            tablei = get_hash(vtable, &argdefs[i]);
            vtable->opts[tablei].argdef = &argdefs[i];
        }
        else
        {
            vtable->stats[i].argdef = &argdefs[i];
        }
    }
}


void vtable_destroy(struct VTable* vtable)
{
    for (size_t i = 0; i < vtable->n_opts; i += 1)
    {
        free(vtable->opts[i].value);
    }
    for (size_t i = 0; i < vtable->n_stats; i += 1)
    {
        free(vtable->stats[i].value);
    }
    free(vtable->opts);
    free(vtable->stats);
}


#endif  /* BARGP_C */
