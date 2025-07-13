#include <bargp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define N_CHANNELS 3


const struct ArgumentDefinition argdefs[] = {
    { .name = "bg-color", .key = 'b', .type =   LONG, .is_optional = true },
    { .name =   "square", .key = 's', .type = STRING, .is_optional = true, .is_list = true },
    { .name =      "out", .type = STRING },
    { .name =    "width", .type =   LONG },
    { .name =   "height", .type =   LONG },
    0
};


struct arguments {
    char* out;
    size_t width;
    size_t height;
    uint8_t bg_color;
};


int main(int argc, char** argv)
{
    FILE* fh = NULL;
    uint8_t* ppm_buf = NULL;
    void* value = NULL;
    size_t buf_size = 0;
    size_t total_args = 0;
    size_t n_opt_args = 0;
    int parse_res = BARGP_SUCCESS;
    struct arguments args = { 0 };
    struct VTable vtable;


    args.bg_color = 0;

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


    args.out = (char*)get_arg_index(&vtable, 0);
    args.width = *(size_t*)get_arg_index(&vtable, 1);
    args.height = *(size_t*)get_arg_index(&vtable, 2);
    if ((value = get_arg_key(&vtable, 'b')) != NULL) args.bg_color = *(char*)(value);

    if ((fh = fopen(args.out, "w")) != NULL)
    {
        fprintf(fh, "%s\n", "P3");
        fprintf(fh, "%lu %lu\n", args.width, args.height);
        fprintf(fh, "%u\n", 255);
        buf_size = args.width * args.height * N_CHANNELS;
        ppm_buf = malloc(sizeof(char) * buf_size);
        for (size_t i = 0; i < buf_size; i += 1)
        {
            ppm_buf[i] = args.bg_color;
        }
        for (size_t i = 0; i < buf_size; i += 1)
        {
            fprintf(fh, "%u\n", ppm_buf[i]);
        }
        fclose(fh);
    }
    else
    {
        fprintf(stderr, "Could not open PPM file!\n");
    }

exit:
    vtable_destroy(&vtable);

    return parse_res;
}
