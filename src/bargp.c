#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


size_t __count_args(const struct ArgumentDefinition* argdefs)
{
    size_t i = 0;
    size_t expected_nargs = 0;


    while (argdefs[i].type != 0)
    {
        expected_nargs += 1;
        i += 1;
    }

    return expected_nargs;
}


void __get_poses(struct RecvArgs* poses, const char** argv, const int argc)
{
    size_t n_pos_args = 0;


    for (size_t i = argc - 1; i > 0; i -= 1)
    {
        if (argv[i - 1][0] != '-')
        {
            n_pos_args += 1;
        }
        else
        {
            break;
        }
    }
    poses->size = n_pos_args;
    poses->values = (char**)malloc(sizeof(char*) * n_pos_args);

    for (size_t i = 0; i < n_pos_args; i += 1)
    {
        poses->values[n_pos_args - i - 1] = argv[argc - i - 1];
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

    return (nametol + argdef->key) % vtable->size;
}


int parse_args(
        struct VTable* vtable,
        const int argc,
        const char** argv,
        const struct ArgumentDefinition* argdefs
) {
    const size_t nargs = __count_args(argdefs);
    size_t tablei;
    struct RecvArgs shorts = { 0 };  // values with keys
    struct RecvArgs longs = { 0 };  // values with names
    struct RecvArgs poses = { 0 };  // positionals


    if (argc - 1 < nargs)
    {
        return BARGP_TOO_FEW_ARGUMENTS;
    }
    else if (nargs < argc - 1)
    {
        return BARGP_TOO_MANY_ARGUMENTS;
    }
    vtable->size = nargs;
    vtable->values = (void**)malloc(sizeof(void*) * nargs);

    __get_poses(&poses, argv, argc);
    printf("Positional arguments (size=%lu) = { %s", poses.size, poses.values[0]);
    for (size_t i = 1; i < poses.size; i += 1)
    {
        printf(", %s", poses.values[i]);
    }
    printf(" }\n");

    for (size_t i = 0; i < argc - 1; i += 1)
    {
        tablei = get_hash(vtable, &argdefs[i]);
        switch (argdefs[i].type)
        {
            case LONG:
                break;
            case DOUBLE:
                break;
            case STRING:
                break;
        }
    }

    return BARGP_SUCCESS;
}


#endif  /* BARGP_C */
