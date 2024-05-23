#ifndef MSL_ALGORITHM_H
#define MSL_ALGORITHM_H

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void msl_swap(void *a, void *b, size_t s);
static void *msl_find(void *begin, void *end, size_t s, void *val);
static uint32_t msl_map(int32_t val, int32_t from_low, int32_t from_high, int32_t to_low, int32_t to_high);
static uint64_t msl_map_long(int64_t val, int64_t from_low, int64_t from_high, int64_t to_low, int64_t to_high);

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

static uint32_t msl_map(int32_t val, int32_t from_low, int32_t from_high, int32_t to_low, int32_t to_high)
{
    return (val - from_low) * (to_high - to_low) / (from_high - from_low) + to_low; 
}

static uint64_t msl_map_long(int64_t val, int64_t from_low, int64_t from_high, int64_t to_low, int64_t to_high)
{
    return (val - from_low) * (to_high - to_low) / (from_high - from_low) + to_low; 
}

#endif
