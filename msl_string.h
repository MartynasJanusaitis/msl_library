#ifndef MSL_STRING_H
#define MSL_STRING_H

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

typedef char* msl_string;

typedef struct msl_string_header
{
    size_t length;
    size_t capacity;
}msl_string_header;

static msl_string_header* msl_string_get_header(msl_string str);
static msl_string msl_string_make_length(char* cstr, size_t length);
static msl_string msl_string_make(char* str);
static void msl_string_destroy(msl_string string);
static msl_string msl_string_concat(msl_string str1, msl_string str2);
static size_t msl_string_length(msl_string str);
static size_t msl_string_capacity(msl_string str);
static msl_string msl_string_erase(msl_string str, int index, int count);
static msl_string msl_string_substring(msl_string str, int index, int count);

static msl_string_header* msl_string_get_header(msl_string str)
{
    return (msl_string_header*)str - 1;
}

static msl_string msl_string_make_length(char* cstr, size_t length)
{
    msl_string_header *header;
    msl_string str;
    size_t header_size = sizeof(msl_string_header);
    void *p = malloc(header_size + length + 1);
    if(cstr == NULL) memset(p, 0, header_size + length + 1);
    
    str = (char*)p + header_size;
    header = msl_string_get_header(str);
    header->capacity = length;
    header->length = length;

    if(length != 0 && cstr != NULL)
        memcpy(str, cstr, length);
    str[length] = '\0';

    return str; 
}

static msl_string msl_string_make(char* str)
{
    return msl_string_make_length(str, strlen(str));
}

static void msl_string_destroy(msl_string string)
{
    free(msl_string_get_header(string));
}

static msl_string msl_string_concat(msl_string str1, msl_string str2)
{
    size_t length1 = msl_string_get_header(str1)->length;
    size_t length2 = msl_string_get_header(str2)->length;
    size_t length3 = length1 + length2; 

    msl_string str3 = msl_string_make_length(NULL, length3);
    
    memcpy(str3, str1, length1);
    memcpy(str3 + length1, str2, length2 + 1);

    return str3;
}

static size_t msl_string_length(msl_string str)
{
    return msl_string_get_header(str)->length;
}

static size_t msl_string_capacity(msl_string str)
{
    return msl_string_get_header(str)->capacity;
}

static msl_string msl_string_erase(msl_string str, int index, int count)
{
    size_t length = msl_string_length(str);
    msl_string newStr = msl_string_make_length(NULL, length - count);
    
    memcpy(newStr, str, index);
    memcpy(newStr + index, str + index + count, length - index - count);
    
    return newStr;
}

static msl_string msl_string_substring(msl_string str, int index, int count)
{
    return msl_string_make_length((char*)str + index, count);
}

#endif
