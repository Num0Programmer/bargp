#include <bargp.h>
#include <stdio.h>
#include <stdlib.h>


const struct ArgumentDefinition argdefs[] = {
    { .name = "bg-color", .key = 'b', .type =   LONG, .is_optional = true },
    { .name =      "out", .type = STRING },
    { .name =    "width", .type =   LONG },
    { .name =   "height", .type =   LONG },
    0
};


int main(int argc, char** argv)
{
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

    vtable_create(&vtable, total_args, n_opt_args, argdefs);
    parse_res = parse_args(&vtable, argc, (const char**)(argv));
    if (parse_res != BARGP_SUCCESS)
    {
        fprintf(stderr, "Parsing failed: %d\n", parse_res);
        goto exit;
    }

exit:
    vtable_destroy(&vtable);

    return parse_res;
}
