#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "http.h"

#define UNUSED(x) (void)x

#ifndef HTTP_BASIC_UATH_USER
#define HTTP_BASIC_UATH_USER "user"
#endif

/**
 * trim_whitespace removes any white space on 
 * the given string.
 */
static char*
trim_whitespace(char *str)
{
    while(isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == 0) {
        return str;
    }

    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';

    return str;
}

/**
 * slice_str removes a portion of the given string.
 */
static void
slice_str(const char *str, char *buffer, size_t start, size_t end)
{
    size_t j = 0;

    for (size_t i = start; i <= end; ++i) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}

char*
get_header_value(const char *header, char *key)
{
    if (!header || !header[0]) {
        return NULL;
    }
    if (!key || !key[0]) {
        return NULL;
    }

    char *tmp = malloc(strlen(header)+2);
    strcpy(tmp, header);
    size_t header_len = strlen(header);
    char *value = malloc(header_len+1);
    slice_str(header, value, strlen(key)+2, header_len+1);

    return value;
}
