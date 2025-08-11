#include <bargp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define N_CHANNELS 3


const char* author = "Num0Programmer";
const char* title = "PPM Stencil";
const char* version = "0.1";
const char* usage = "./ppm [OPTIONS]... OUT WIDTH HEIGHT";
const char* desc = "Stencil application - produces a PPM image OUT of WIDTH and HEIGHT size";
const char* postamble = "Report bugs to num0programmer@no.domain";
const struct ArgumentDefinition argdefs[] = {
    {
        .name        = "bg-color",
        .key         = 'b',
        .type        = LONG,
        .is_optional = true,
        .desc        = "Initializes the image with a solid color",
    },
    {
        .name        = "rect",
        .key         = 'r',
        .type        = LONG,
        .is_optional = true,
        .is_list     = true,
        .desc        = "Draws a rectangle",
    },
    { .name =      "out", .type = STRING, .desc =              "Name of PPM file" },
    { .name =    "width", .type =   LONG, .desc =  "Width of the image in pixels" },
    { .name =   "height", .type =   LONG, .desc = "Height of the image in pixels" },
    0
};


struct Rect {
    size_t pos[2];  // upper left corner of shape
    size_t width;
    size_t height;
    size_t color[3];  // RGB [0-255]
};


struct Arguments {
    char* out;
    size_t width;
    size_t height;
    uint8_t bg_color;
};


void draw_rect(uint8_t* buf, const size_t width, const size_t height, const struct Rect* rect)
{
    for (size_t i = rect->pos[1]; i < rect->pos[1] + rect->height && i < height; i += 1)
    {
        // printf("Accessing row = %lu\n", i);
        for (size_t j = rect->pos[0];
            j < rect->pos[0] + rect->width * N_CHANNELS && j < width * N_CHANNELS;
            j += N_CHANNELS)
        {
            // printf("Accessing col = %lu\n", j);
            buf[i * (width * N_CHANNELS) + j] = rect->color[0];
            buf[i * (width * N_CHANNELS) + j + 1] = rect->color[1];
            buf[i * (width * N_CHANNELS) + j + 2] = rect->color[2];
        }
    }
}


int main(int argc, char** argv)
{
    FILE* fh = NULL;
    uint8_t* ppm_buf = NULL;
    void* value = NULL;
    size_t buf_size = 0;
    int parse_res = BARGP_SUCCESS;
    struct Arguments args = { 0 };
    struct Rect* rect = NULL;


    args.bg_color = 0;

    parse_res = parse_args(argc, (const char**)(argv), argdefs);
    if (parse_res != BARGP_SUCCESS)
    {
        fprintf(stderr, "Parsing failed: %d\n", parse_res);
        goto exit;
    }


    args.out = (char*)get_arg_index(0);
    args.width = *(size_t*)get_arg_index(1);
    args.height = *(size_t*)get_arg_index(2);
    if ((value = get_arg_key('b')) != NULL) args.bg_color = *(char*)(value);
    if ((value = get_arg_key('r')) != NULL)
    {
        rect = malloc(sizeof(struct Rect));
        rect->pos[0] = ((size_t*)(value))[0];
        rect->pos[1] = ((size_t*)(value))[1];
        rect->width = ((size_t*)(value))[2];
        rect->height = ((size_t*)(value))[3];
        rect->color[0] = ((size_t*)(value))[4];
        rect->color[1] = ((size_t*)(value))[5];
        rect->color[2] = ((size_t*)(value))[6];
    }

    if ((fh = fopen(args.out, "w")) != NULL)
    {
        fprintf(fh, "%s\n", "P3");
        fprintf(fh, "%lu %lu\n", args.width, args.height);
        fprintf(fh, "%u\n", 255);
        buf_size = args.width * args.height * N_CHANNELS;
        ppm_buf = malloc(sizeof(uint8_t) * buf_size);
        for (size_t i = 0; i < buf_size; i += 1)
        {
            ppm_buf[i] = args.bg_color;
        }
        if (rect != NULL) draw_rect(ppm_buf, args.width, args.height, rect);
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
    free_argtable();

    return parse_res;
}
