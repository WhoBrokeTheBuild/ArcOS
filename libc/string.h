#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include <stdlib.h>

size_t strlen(const char * str);
int strcmp(const char * str1, const char *str2);

void * memcpy(void * dest, void * src, size_t len);

#endif // LIBC_STRING_H
