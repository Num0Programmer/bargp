#include <bargp.h>
#include <stdio.h>
#include <stdlib.h>


const struct ArgumentDefinition argdefs[] = {
    { .name =    "out", .type = STRING },
    { .name =  "width", .type =   LONG },
    { .name = "height", .type =   LONG },
    0
};


int main(int argc, char** argv) {
    int parse_res = BARGP_SUCCESS;
    struct VTable vtable;


    parse_res = parse_args(
        &vtable,
        argc,
        (const char**)(argv),
        (const struct ArgumentDefinition*)(&argdefs)
    );
    if (parse_res != BARGP_SUCCESS)
    {
        printf("Parsing failed: %d\n", parse_res);
        goto finish;
    }
    // printf("args.outfile = %s\n", args.outfile);
    // printf("args.width = %lu\n", args.width);
    // printf("args.height = %lu\n", args.height);

finish:
    return parse_res;
}
