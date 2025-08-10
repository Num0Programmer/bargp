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
