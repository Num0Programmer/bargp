#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


void parse_args(
        void* args,
        const int argc,
        const char** argv,
        const struct ArgumentDefinition* argdefs
) {
    for (size_t i = 0; i < argc; i += 1)
    {
        switch (argdefs[i].type)
        {
            case CHAR:
                break;
            case LONG:
                break;
            case DOUBLE:
                break;
            case STRING:
                *(char**)(args + (i * 8)) = argv[i + 1];
                break;
            default:
                break;
        }
    }
}


#endif  /* BARGP_C */
