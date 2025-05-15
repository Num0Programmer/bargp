#include <bargp.h>
#include <stdio.h>
#include <stdlib.h>


const struct ArgumentDefinition argdefs[] = {
    { .name = "out", .type = STRING },
    { .name = "width", .type = LONG },
    { .name = "height", .type = LONG },
};


struct Arguments {
    char* outfile;
    unsigned int width;
    unsigned int height;
};


int main(int argc, char** argv) {
    struct Arguments args;


    // printf("&args = %lu\n", &args);
    // printf("&args.outfile = %lu\n", &args.outfile);
    // printf("&args.width = %lu\n", &args.width);
    // printf("&args.height = %lu\n", &args.height);
    parse_args(
        (void*)(&args),
        argc,
        (const char**)(argv),
        (const struct ArgumentDefinition*)(&argdefs)
    );

    return 0;
}
