#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#define ERROR_EXIT(...) do { fprintf(stderr, __VA_ARGS__); } while (0)

#define ERROR_RETURN(R, ...) do { fprintf(stderr, __VA_ARGS__); return R; } while (0)

#endif
