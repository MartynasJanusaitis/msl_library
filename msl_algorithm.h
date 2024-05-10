#ifndef MSL_ALGORITHM_H
#define MSL_ALGORITHM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void msl_swap(void *a, void *b, size_t s)
{
    void *temp = malloc(s);
    memcpy(temp, a, s);
    memcpy(a, b, s);
    memcpy(b, temp, s);
    free(temp);
}

static void *msl_find(void *begin, void *end, size_t s, void *val)
{
    void *p;

    for(p = begin; p < end; p = (char*)p + s)
    {
        if(strncmp((char*)val, (char*)p, s) == 0) break;        
    }

    return p;
}

#endif
