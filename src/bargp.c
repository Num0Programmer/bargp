#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


void* __resolve_type(const char* value, const struct ArgumentDefinition* argdef)
{
    void* mem = NULL;


    switch(argdef->type)
    {
        case LONG:
            mem = malloc(sizeof(long));
            *(long*)(mem) = strtol(value, NULL, 10);
            break;
        case DOUBLE:
            mem = malloc(sizeof(double));
            *(double*)(mem) = strtod(value, NULL);
            break;
        case STRING:
            mem = malloc(sizeof(char) * strlen(value));
            strcpy(mem, value);
            break;
        default:
            break;
    }

    return mem;
}


void count_args(size_t* total_args, size_t* n_opt_args, const struct ArgumentDefinition* argdefs)
{
    size_t i = 0;
    *total_args = 0;
    *n_opt_args = 0;


    while(argdefs[i].type != 0)
    {
        if (argdefs[i].is_optional)
        {
            *n_opt_args += 1;
            *total_args += 1;
        }
        *total_args += 1;
        i += 1;
    }
}


void* get_arg_index(const struct VTable* vtable, const size_t index)
{
    return vtable->stats[index].value;
}


void* get_arg_key(const struct VTable* vtable, const char key)
{
    return vtable->keystoargs[get_hash_key(vtable, key)]->value;
}


void* get_arg_name(const struct VTable* vtable, const char* name)
{
    return vtable->namestoargs[get_hash_name(vtable, name)].value;
}


size_t get_hash_key(const struct VTable* vtable, const char key)
{
    return key % vtable->n_opt_keys;
}


size_t get_hash_name(const struct VTable* vtable, const char* name)
{
    const size_t name_len = strlen(name);
    size_t nametol = 0;


    for (size_t i = 0; i < name_len; i += 1)
    {
        nametol += name[i];
    }

    return nametol % vtable->n_opt_names;
}


int parse_args(
        struct VTable* vtable,
        const int argc,
        const char** argv
) {
    size_t tablei;
    size_t statsi = 0;


    for (size_t i = 1; i < argc; i += 1)
    {
        // try optional argument
        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            tablei = get_hash_name(vtable, &argv[i][2]);
            vtable->namestoargs[tablei].value = __resolve_type(
                argv[i + 1],
                vtable->namestoargs[tablei].argdef
            );
            i += 1;
        }
        else if (argv[i][0] == '-')
        {
            tablei = get_hash_key(vtable, argv[i][1]);
            vtable->keystoargs[tablei]->value = __resolve_type(
                argv[i + 1],
                vtable->keystoargs[tablei]->argdef
            );
            i += 1;
        }
        // try static argument
        else
        {
            vtable->stats[statsi].value = __resolve_type(
                argv[i],
                vtable->stats[statsi].argdef
            );
            statsi += 1;
        }
    }

    return BARGP_SUCCESS;
}


void vtable_create(
        struct VTable* vtable,
        const size_t total_args,
        const size_t n_opt_args,
        const struct ArgumentDefinition* argdefs
) {
    size_t tablei;
    size_t statsi = 0;


    vtable->n_opt_keys = BARGP_N_CHARS_ALPHA - 1;  // removes '-'
    vtable->n_opt_names = BARGP_MAX_NAME_LEN * BARGP_N_CHARS_ALPHA;
    vtable->n_stats = total_args - n_opt_args;
    vtable->keystoargs = malloc(sizeof(struct ArgDefToValue) * vtable->n_opt_keys);
    vtable->namestoargs = malloc(sizeof(struct ArgDefToValue) * vtable->n_opt_names);
    vtable->stats = malloc(sizeof(struct ArgDefToValue) * vtable->n_stats);
    for (size_t i = 0; i < total_args; i += 1)
    {
        if (argdefs[i].is_optional)
        {
            tablei = get_hash_name(vtable, argdefs[i].name);
            vtable->namestoargs[tablei].argdef = &argdefs[i];

            if (argdefs[i].key)
            {
                vtable->keystoargs[get_hash_key(vtable, argdefs[i].key)] =
                    &vtable->namestoargs[tablei];
            }
        }
        else
        {
            vtable->stats[statsi].argdef = &argdefs[i];
            statsi += 1;
        }
    }
}


void vtable_destroy(struct VTable* vtable)
{
    for (size_t i = 0; i < vtable->n_opt_names; i += 1)
    {
        free(vtable->namestoargs[i].value);
    }
    for (size_t i = 0; i < vtable->n_stats; i += 1)
    {
        free(vtable->stats[i].value);
    }
    free(vtable->keystoargs);
    free(vtable->namestoargs);
    free(vtable->stats);
}


#endif  /* BARGP_C */
