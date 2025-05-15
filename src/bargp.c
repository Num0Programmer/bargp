#ifndef BARGP_C
#define BARGP_C

#include "../include/bargp.h"


void parse_args(
    void* args,
    const int argc,
    const char** argv,
    const struct ArgumentDefinition* argdefs
) {
    printf("args.outfile = %s\n", *(char**)(args));
}


#endif  /* BARGP_C */
