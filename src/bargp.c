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


int parse_args(
        struct VTable* vtable,
        const int argc,
        const char** argv,
        const struct ArgumentDefinition* argdefs
) {
    size_t nargs = __count_args(argdefs);


    if (argc - 1 < nargs)
    {
        return BARGP_TOO_FEW_ARGUMENTS;
    }
    else if (nargs < argc - 1)
    {
        return BARGP_TOO_MANY_ARGUMENTS;
    }
    vtable->size = nargs;
    vtable->values = (void*)malloc(sizeof(void*) * nargs);

    return BARGP_SUCCESS;
}


#endif  /* BARGP_C */
