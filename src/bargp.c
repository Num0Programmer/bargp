#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


void* __parse_value(const char* value, const struct ArgumentDefinition* argdef)
{
    void* mem = NULL;


    switch (argdef->type)
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


void* __parse_list(const char* value, const struct ArgumentDefinition* argdef)
{
    size_t count = 1;
    char* tok = NULL;
    void* mem = NULL;
    size_t i = 0;


    while (value[i] != '\0')
    {
        if (value[i] == *BARGP_LIST_DELIM) count += 1;
        i += 1;
    }
    // printf("Number of items in '%s' = %lu\n", value, count);
    switch (argdef->type)
    {
        case LONG:
            mem = malloc(sizeof(long) * count);
            break;
        case DOUBLE:
            mem = malloc(sizeof(double) * count);
            break;
        case STRING:
            fprintf(stderr, "Parsing lists of strings is not yet supported!\n");
            exit(83);
            break;
    }

    tok = strtok((char*)(value), BARGP_LIST_DELIM);
    for (i = 0; i < count && tok != NULL; i += 1)
    {
        switch (argdef->type)
        {
            case LONG:
                *(long*)(mem + i * 8) = strtol(tok, NULL, 10);
                break;
            case DOUBLE:
                *(double*)(mem + i * 8) = strtod(tok, NULL);
                break;
            case STRING:
                // TODO: calculate sum of length of strings and allocate that many characters -
                // leave in null characters or something
                // OTHERWISE: figure out how different 'void*' is from 'void**'
                break;
        }
        tok = strtok(NULL, BARGP_LIST_DELIM);
    }

    return mem;
}


void count_args(
        size_t* total_args,
        size_t* n_opt_args,
        const struct ArgumentDefinition* argdefs,
        const char** argv,
        const int argc
) {
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

    if ((size_t)(argc) - 1 + *n_opt_args < *total_args - *n_opt_args)
    {
        fprintf(
            stderr,
            "Too few arguments! Expected %lu, but received %lu\n",
            *total_args - *n_opt_args,
            (size_t)(argc - 1 + n_opt_args)
        );
        exit(BARGP_TOO_FEW_ARGUMENTS);
    }
    else if (*total_args - *n_opt_args < (size_t)(argc) - 1 - *n_opt_args)
    {
        fprintf(
            stderr,
            "Too many arguments! Expected %lu, but received %d\n",
            *total_args - *n_opt_args,
            argc - 1
        );
        exit(BARGP_TOO_MANY_ARGUMENTS);
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


void help_fmt()
{
    exit(0);
}


int parse_args(
        struct VTable* vtable,
        const int argc,
        const char** argv,
        const struct ArgumentDefinition* argdefs
) {
    size_t tablei;
    size_t statsi = 0;
    size_t total_args = 0;
    size_t n_opt_args = 0;
    const struct ArgumentDefinition* argdef = NULL;


    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) help_fmt();
    count_args(&total_args, &n_opt_args, argdefs, argv, argc);
    vtable_create(vtable, total_args, n_opt_args, argdefs);

    for (size_t i = 1; i < argc; i += 1)
    {
        // try optional argument
        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            tablei = get_hash_name(vtable, &argv[i][2]);
            argdef = vtable->namestoargs[tablei].argdef;

            if (argdef->is_list)
            {
                vtable->namestoargs[tablei].value = __parse_list(argv[i + 1], argdef);
            }
            else
            {
                vtable->namestoargs[tablei].value = __parse_value(argv[i + 1], argdef);
            }
            i += 1;
        }
        else if (argv[i][0] == '-')
        {
            tablei = get_hash_key(vtable, argv[i][1]);
            argdef = vtable->keystoargs[tablei]->argdef;

            if (argdef->is_list)
            {
                vtable->keystoargs[tablei]->value = __parse_list(argv[i + 1], argdef);
            }
            else
            {
                vtable->keystoargs[tablei]->value = __parse_value(argv[i + 1], argdef);
            }
            i += 1;
        }
        // try static argument
        else
        {
            argdef = vtable->stats[statsi].argdef;

            if (argdef->is_list)
            {
                vtable->stats[statsi].value = __parse_list(argv[i], argdef);
            }
            else
            {
                vtable->stats[statsi].value = __parse_value(argv[i], argdef);
            }
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
