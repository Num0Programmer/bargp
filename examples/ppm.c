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
    size_t total_args = 0;
    size_t n_opt_args = 0;
    int parse_res = BARGP_SUCCESS;
    struct VTable vtable;


    count_args(&total_args, &n_opt_args, argdefs);
    if (argc - 1 + n_opt_args < total_args - n_opt_args)
    {
        fprintf(
            stderr,
            "Too few arguments! Expected %lu, but received %lu\n",
            total_args - n_opt_args,
            (size_t)(argc - 1 + n_opt_args)
        );
        return BARGP_TOO_FEW_ARGUMENTS;
    }
    else if (total_args - n_opt_args < argc - 1 - n_opt_args)
    {
        fprintf(
            stderr,
            "Too many arguments! Expected %lu, but received %d\n",
            total_args - n_opt_args,
            argc - 1
        );
        return BARGP_TOO_MANY_ARGUMENTS;
    }

    vtable_create(&vtable, total_args, argdefs);
    parse_res = parse_args(
        &vtable,
        argc,
        (const char**)(argv),
        total_args,
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
    // printf("bg-color = %lu\n", *(unsigned long*)get_arg(&vtable, &argdefs[3]));

exit:
    vtable_destroy(&vtable);

    return parse_res;
}
