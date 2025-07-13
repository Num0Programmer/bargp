#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


void** __resolve_type(const char* value, const struct ArgumentDefinition* argdef)
{
    void** alloc = malloc(sizeof(void*));


    switch(argdef->type)
    {
        case LONG:
            *alloc = malloc(sizeof(double));
            **(long**)(alloc) = strtol(value, NULL, 10);
            break;
        // case DOUBLE:
        //     alloc = (void*)malloc(sizeof(double));
        //     *(double*)alloc = strtod(value, NULL);
        //     break;
        case STRING:
            *alloc = malloc(sizeof(char) * strlen(value));
            strcpy(*alloc, value);
            break;
        default:
            break;
    }

    return alloc;
}


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
    if (argdef->is_optional)
    {
        return vtable->opts[get_hash(vtable, argdef)].value;
    }

    for (size_t i = 0; i < vtable->n_stats; i += 1)
    {
        if (argdef == vtable->stats[i].argdef)
        {
            return vtable->stats[i].value;
        }
    }
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


int parse_args(
        struct VTable* vtable,
        const int argc,
        const char** argv,
        const size_t total_args,
        const struct ArgumentDefinition* argdefs
) {
    size_t size;
    size_t tablei;
    size_t statsi = 0;


    for (size_t i = 1; i < argc; i += 1)
    {
        // try optional argument
        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            // for (size_t j = 0; j < total_args; j += 1)
            // {
            //     if (argdefs[j].key == argv[i][1])
            //     {
            //         tablei = get_hash(vtable, &argdefs[j]);
            //         vtable->opts[tablei].argdef = &argdefs[j];
            //         vtable->opts[tablei].value = __resolve_type(argv[i + 1], &argdefs[j]);
            //     }
            // }

            // i += 1;
        }
        else if (argv[i][0] == '-')
        {
            printf("Parsing key flags is not implemented!\n");
            // for (size_t j = 0; j < total_args; j += 1)
            // {
            //     if (argdefs[j].key == argv[i][1])
            //     {
            //     }
            // }

            i += 1;
        }
        // try static argument
        else
        {
            size = strlen(argv[i]);
            vtable->stats[statsi].value = calloc(size, sizeof(char));
            strcpy((char*)(vtable->stats[statsi].value), argv[i]);
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
    size_t statsi = 0;
    size_t tablei;


    vtable->n_stats = total_args - n_opt_args;
    vtable->n_opts = BARGP_MAX_NAME_LEN * BARGP_N_CHARS_ALPHA;
    vtable->opts = (struct ArgDefToValue*)malloc(sizeof(struct ArgDefToValue) * vtable->n_opts);
    vtable->stats = (struct ArgDefToValue*)malloc(sizeof(struct ArgDefToValue) * vtable->n_stats);
    for (size_t i = 0; i < total_args; i += 1)
    {
        if (argdefs[i].is_optional)
        {
            tablei = get_hash(vtable, &argdefs[i]);
            vtable->opts[tablei].argdef = &argdefs[i];
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
