# Better Argument Parser (bargp)

A general purpose, cross-platform argument parser for terminal applications written in C.

## Install
```sh
# clone repo

sudo sh install.sh
```
By default, this installs the header and shared library to '/usr/local'.

### Changing Installation Directory
If you do not maintain root access to your system - for example you are building on a HPC cluster - you may specify where the library is installed with the following:
```sh
sh install.sh /path/to/install
```

## Linking
No matter where you have installed the library, you will have to link using '-lbargp'. If you have root access, then compiling with the following is all which is required:
```sh
gcc program.c -o program -lbargp
```

If you have installed the library to different directory, then a few options are available.

### Option 1
Explicity specify where to find the header and shared library using the '-I' and '-L' options.
```sh
-I/path/to/install/include
-L/path/to/install/lib
```
'-I' will tell GCC which directory to search for the header file 'bargp.h'. Similarly, '-L' will tell GCC where to search for the shared library. The final command will take the following form:
```sh
gcc -I/path/to/install/include -L/path/to/install/lib program.c -o program
```

However, you can avoid the above command by setting the 'C_INCLUDE_PATH' and 'LD_LIBRARY_PATHS' so GCC will automatically search the custom directory by doing the following:
```sh
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/path/to/install/include
```
This will remove the necessity to specify the location of the header with the '-I' option.

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/install/lib
```
This will remove the necessity to specify the location of the shared library with the '-L' option.

However, you will still be required to link with '-lbargp', but the final command will take the following form:
```sh
gcc program.c -o program -lbargp
```

## Quick Start
### Information About Your Program
A few lines are required before any parsing can be done. In a new file, specify the 'title' and 'version' of your program. You can define the 'usage' which shows how to call your program from the terminal. The 'desc' or description, which provides a little information about your program. The 'postamble' which allows you to include any extra information at the end of the help message. This could be information such as where to submit bug reports.
```c
#include <bargp.h>


const char* author = "Your Name";
const char* title = "Your Program Name";
const char* version = "0.1";
const char* usage = "./program [OPTIONS]... [ARGUMENTS]...";
const char* desc = "What your program does";
const char* postamble = "Other information";
```
If you do not have anything to put in the postamble, then you can set it to an empty string.

### Specifying Arguments
You can provide information about the arguments your program accepts with an array of 'ArgumentDefinition' structs. The following example demonstrates how to specify a positional argument and an optional argument.
```c
const struct ArgumentDefinition argdefs[] = {
    {
        .name        = "optional-arg",
        .key         = 'l',
        .type        = LONG,
        .is_optional = true,
        .desc        = "Information about your option",
    }
    {
        .name = "positional-arg",
        .type = STRING,
        .desc = "Information about this positional argument",
    }
    0
};
```
This means the program accepts a total of 2 arguments, but will exit if a value for the 'positional-arg' is not provided. Please refer to the [ArgumentDefinition Specs]() for more information on the fields of this structure and how they are used.

### Parsing Arguments
No you are ready to parse arguments! All which is required is initializing a table into which the arguments and their values can be stored and calling the 'parse_args()' function. Below if a sample main program to demonstrate how to initialize your program and parse the arguments:
```c
int main(int argc, char** argv)
{
    int parse_res = BARGP_SUCCESS;
    struct VTable vtable = { 0 };


    parse_res = parse_args(&vtable, argc, (const char**)(argv), argdefs);
    if (parse_res != BARGP_SUCCESS)
    {
        fprintf(stderr, "Parsing failed: %d\n", parse_res);
        goto exit;
    }

exit:
    vtable_destroy(&vtable);

    return parse_res;
}
```
Notice, very little is done in 'main()' to accomplish parsing. This is because BARGP takes care of checking that the expected arguments were passed and constructing the table of arguments. The only actions *required* of the programmer is to check if parsing succeeded, and releasing the memory allocated for the table before exiting the program. At this point, your program is ready to be published.

If you are looking to accept a number of arguments from the user, you can examine the [PPM stencil example](./examples/ppm.c) to see how values from the user can be extracted from the table. Although it is not required, it might be helpful to store all the values passed by the user in a structure directly after argument parsing. This reduces the amount of effort for extracting a value; additionally, you can immediately free the memory for the table if space is tight for your program.
