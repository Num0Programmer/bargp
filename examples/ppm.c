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


    parse_args(&args, argc, argv, &argdefs);

    return 0;
}
