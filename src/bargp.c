#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


void __get_longs(struct RecvArgs* longs, const char** argv, const int argc)
{
    size_t i = 0;
    size_t n_long_args = 0;


    for (i = 1; i < argc; i += 1)
    {
        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            n_long_args += 1;
        }
    }
    longs->size = n_long_args;
    longs->values = (char**)malloc(sizeof(char*) * n_long_args);

    i = 0;
    while (i < n_long_args)
    {
        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            longs->values[i] = (char*)malloc(sizeof(char) * strlen(argv[i]));
            strcpy(longs->values[i], argv[i]);
            i += 1;
        }
    }
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
        poses->values[n_pos_args - i - 1] =
            (void*)malloc(sizeof(char) * strlen(argv[argc - i - 1]));
        strcpy(poses->values[n_pos_args - i - 1], argv[argc - i - 1]);
    }
}


size_t count_args(const struct ArgumentDefinition* argdefs)
{
    size_t i = 0;
    size_t expected_n_args = 0;


    while (argdefs[i].type != 0 && !argdefs[i].is_optional)
    {
        expected_n_args += 1;
        i += 1;
    }

    return expected_n_args;
}


void* get_arg(const struct VTable* vtable, const struct ArgumentDefinition* argdef)
{
    const size_t tablei = get_hash(vtable, argdef);


    return vtable->values[tablei];
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
    size_t n_args_parsed = 0;
    struct RecvArgs shorts = { 0 };  // values with keys
    struct RecvArgs longs = { 0 };  // values with names
    struct RecvArgs poses = { 0 };  // positionals


    __get_poses(&poses, argv, argc);
    __get_longs(&longs, argv, argc);

    // parse positional arguments
    for (size_t i = 0; i < poses.size; i += 1)
    {
        // TODO: will have to add the number of short and long flag arguments
        tablei = get_hash(vtable, &argdefs[i]);
        switch (argdefs[i].type)
        {
            case LONG:
                vtable->values[tablei] = (void*)malloc(sizeof(long));
                *(long*)vtable->values[tablei] = strtol(poses.values[i], NULL, 10);
                break;
            case DOUBLE:
                vtable->values[tablei] = (void*)malloc(sizeof(double));
                *(double*)vtable->values[tablei] = strtod(poses.values[i], NULL);
                break;
            case STRING:
                vtable->values[tablei] = (void*)malloc(sizeof(char) * strlen(poses.values[i]));
                strcpy(vtable->values[tablei], poses.values[i]);
                break;
        }
    }

    for (size_t i = 0; i < shorts.size; i += 1)
    {
        free(shorts.values[i]);
    }
    free(shorts.values);
    free(shorts.codes);
    for (size_t i = 0; i < longs.size; i += 1)
    {
        free(longs.values[i]);
    }
    free(longs.values);
    free(longs.codes);
    for (size_t i = 0; i < poses.size; i += 1)
    {
        free(poses.values[i]);
    }
    free(poses.values);
    free(poses.codes);
    return BARGP_SUCCESS;
}


void vtable_create(struct VTable* vtable, const size_t size)
{
    vtable->size = size;
    vtable->values = (void**)malloc(sizeof(void*) * size);
}


void vtable_destroy(struct VTable* vtable)
{
    for (size_t i = 0; i < vtable->size; i += 1)
    {
        free(vtable->values[i]);
    }
    free(vtable->values);
}


#endif  /* BARGP_C */
