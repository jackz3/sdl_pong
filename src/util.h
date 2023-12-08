#ifndef util_h
#define util_h

#include <stdlib.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int rand_int(int min, int max);

#endif