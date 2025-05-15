#include <bargp.h>
#include <stdio.h>
#include <stdlib.h>


const struct ArgumentDefinition argdefs[] = {
    { .name =    "out", .type = STRING },
    { .name =  "width", .type =   LONG },
    { .name = "height", .type =   LONG },
};


struct Arguments {
    char*  outfile;
    size_t width;
    size_t height;
};


int main(int argc, char** argv) {
    struct Arguments args = { 0 };
    int parse_res = ARG_PARSE_FAILURE;


    parse_res = parse_args(
        (void*)(&args),
        argc,
        (const char**)(argv),
        (const struct ArgumentDefinition*)(&argdefs)
    );
    if (parse_res < ARG_PARSE_SUCCESS)
    {
        printf("Parsing failed: %d\n", parse_res);
    }
    printf("args.outfile = %s\n", args.outfile);
    printf("args.width = %lu\n", args.width);
    printf("args.height = %lu\n", args.height);

    return parse_res;
}
