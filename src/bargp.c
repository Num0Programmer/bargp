#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


int parse_args(
        void* args,
        const int argc,
        const char** argv,
        const struct ArgumentDefinition* argdefs
) {
    for (size_t i = 0; i < argc - 1; i += 1)
    {
        switch (argdefs[i].type)
        {
            case LONG:
                *(size_t*)(args + (i * 8)) = strtol(argv[i + 1], NULL, 10);
                break;
            case DOUBLE:
                *(double*)(args + (i * 8)) = strtod(argv[i + 1], NULL);
                break;
            case STRING:
                *(const char**)(args + (i * 8)) = argv[i + 1];
                break;
            default:
                fprintf(stderr, "Did not parse argument #%lu = '%s'!\n", i + 1, argv[i + 1]);
                return ARG_NOT_PARSED;
        }
    }

    return ARG_PARSE_SUCCESS;
}


#endif  /* BARGP_C */
