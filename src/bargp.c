#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


void parse_args(
    void* args,
    const int argc,
    const char** argv,
    const struct ArgumentDefinition* argdefs
) {
    printf("\nargs.outfile = %s\n", *(char**)(args));
    printf("args = %lu\n", args);
    printf("args + 1 = %lu\n", args + 1);
    printf("args + 8 = %lu\n", args + 8);
    printf("args + 16 = %lu\n", args + 16);
    printf("args.width = %lu\n", *(size_t*)(args + 8));
    printf("args.height = %lu\n", *(size_t*)(args + 16));
}


#endif  /* BARGP_C */
