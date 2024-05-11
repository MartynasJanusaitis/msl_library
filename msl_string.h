#ifndef MSL_STRING_H
#define MSL_STRING_H

#include <stdlib.h>
#include <string.h>

typedef char* msl_string;

typedef struct msl_string_header
{
    size_t length;
    size_t capacity;
}msl_string_header;

static msl_string msl_string_make_length(char* cstring, size_t length)
{
    msl_string_header *header;
    msl_string string;
    size_t header_size = sizeof(msl_string_header);
    void *p = malloc(header_size + length + 1);
    if(cstring == NULL) memset(p, 0, header_size + length + 1);

    string = (msl_string){(char*)p + header_size};
    header = (msl_string_header*)p - 1;
    header->capacity = length;
    header->length = length;

    if(length != 0 && cstring != NULL)
        memcpy(string, cstring, length);

    return string; 
}

static msl_string msl_string_make(char* str)
{
    size_t length = strlen(str);
    return msl_string_make_length(str, length);
}

static msl_string_header* msl_string_get_header(msl_string string)
{
    return (msl_string_header*)string - 1;
}

static void msl_string_destroy(msl_string string)
{
    free(msl_string_get_header(string));
}

#endif
