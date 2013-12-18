#ifndef _STRING_H
#define _STRING_H

#include "types.h"

int strlen(const char *);
int strncmp(const char *, const char *, int);
void *gethexaddr(const char *, const char **);
int getaddrs(void **, void **, const char *);

void *memchr(const void *s, int c, size_t n);
char *strchr(const char *s, int c);

void *memset(void *s, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);
int memcmp(const void *cs, const void *ct, size_t count);

#endif
