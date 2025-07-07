#include <bargp.h>
#include <stdio.h>
#include <stdlib.h>


const struct ArgumentDefinition argdefs[] = {
    { .name =      "out", .type = STRING },
    { .name =    "width", .type =   LONG },
    { .name =   "height", .type =   LONG },
    { .name = "bg-color", .type =   LONG, .is_optional = true },
    0
};


int main(int argc, char** argv) {
    const size_t n_args = count_args(argdefs);
    int parse_res = BARGP_SUCCESS;
    struct VTable vtable;


    if (argc - 1 < n_args)
    {
        fprintf(stderr, "Too few arguments! Expected %lu, but received %d\n", n_args, argc);
        return BARGP_TOO_FEW_ARGUMENTS;
    }
    else if (n_args < argc - 1)
    {
        fprintf(stderr, "Too many arguments! Expected %lu, but received %d\n", n_args, argc);
        return BARGP_TOO_MANY_ARGUMENTS;
    }

    vtable_create(&vtable, n_args);
    parse_res = parse_args(
        &vtable,
        argc,
        (const char**)(argv),
        (const struct ArgumentDefinition*)(&argdefs)
    );
    if (parse_res != BARGP_SUCCESS)
    {
        fprintf(stderr, "Parsing failed: %d\n", parse_res);
        goto exit;
    }
    printf("outfile = %s\n", (char*)get_arg(&vtable, &argdefs[0]));
    printf("width = %lu\n", *(unsigned long*)get_arg(&vtable, &argdefs[1]));
    printf("height = %lu\n", *(unsigned long*)get_arg(&vtable, &argdefs[2]));

exit:
    vtable_destroy(&vtable);

    return parse_res;
}
