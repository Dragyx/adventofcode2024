#ifndef HELPERS_H_
#define HELPERS_H_
#include <stdio.h>

// loads the inputs file from the inputs/ directory
FILE* load_input(unsigned int day);

// https://stackoverflow.com/questions/3437404/min-and-max-in-c
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif
