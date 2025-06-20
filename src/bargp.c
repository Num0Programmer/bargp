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


struct RecvArg* __get_poses(const char** argv, const int argc)
{
    size_t n_pos_args = 0;
    struct RecvArg* poses = NULL;


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
    poses = (struct RecvArg*)malloc(sizeof(struct RecvArg) * n_pos_args);

    for (size_t i = 0; i < n_pos_args; i += 1)
    {
        poses[i].value = argv[argc - i - 1];
    }

    return poses;
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
    struct RecvArg* shorts;  // values with keys
    struct RecvArg* longs;  // values with names
    struct RecvArg* poses;  // positionals


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

    poses = __get_poses(argv, argc);

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
