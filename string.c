#include "string.h"

int hexlut[1 + 'F' - '0'] = {
	0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, /* 0 - 9 */
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0xa, 0xb, 0xc, 0xd, 0xe, 0xf                      /* A - F */
};

int strlen(const char *str)
{
	const char *c = str;

	while (*(c++))
		;

	return c - str;
}

int strncmp(const char *stra, const char *strb, int len)
{
	int diff=0;
	const char *a = stra;
	const char *b = strb;

	while ((a - stra) < len)
		diff += *(a++) - *(b++);

	return diff;
}

void *gethexaddr(const char *str, const char **end)
{
	const char *s = str;
	char *e;
	void *addr = NULL;
	int shift = 0;

	/* set the end */
	while (*s) {
		if (*s == ' ' || *s == '\t' || *s == ',')
			break;
		s++;
	}

	if (!*s)
		return NULL;

	e = (char *)s;
	s = (const char *)(e - 1);

	while (s >= str && *s != 'x') {
		/* we assume base16! */
		int off = (*s > 'F') ? 0x20 : 0x0;
		addr += hexlut[*s - ('0' + off)] << shift;
		shift += 4;
		s--;
	}

	if (end)
		*end = e;

	return addr;
}

int getaddrs(void **k, void **d, const char *str)
{
	const char *s = str;
	const char *end;

	while (*s) {
		if (*s == 'l')
			if (strncmp(s, "loadaddrs=", 10) == 0)
				break;

		s++;
	}

	if (!*s)
		return -1;

	s += 10; /* skip over 'loadaddrs=' */

	/* allow address to be 'appended' for some use cases */
	if (strncmp(s, "appended", 8) == 0) {
		*k = NULL;
		end = s + 8;
	} else
		*k = gethexaddr(s, &end);

	if (*end == ',')
		s = end + 1;
	else
		return -1;

	*d = gethexaddr(s, NULL);

	return 0;
}

/**
 * memchr - Find a character in an area of memory.
 * @s: The memory area
 * @c: The byte to search for
 * @n: The size of the area.
 *
 * returns the address of the first occurrence of @c, or %NULL
 * if @c is not found
 */
void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = s;
	while (n-- != 0) {
		if ((unsigned char)c == *p++) {
			return (void *)(p - 1);
		}
	}
	return NULL;
}

/**
 * strchr - Find the first occurrence of a character in a string
 * @s: The string to be searched
 * @c: The character to search for
 */
char *strchr(const char *s, int c)
{
	for (; *s != (char)c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *)s;
}

/**
 * memset - Fill a region of memory with the given value
 * @s: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 *
 * Do not use memset() to access IO space, use memset_io() instead.
 */
void *memset(void *s, int c, size_t count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void *memcpy(void *dest, const void *src, size_t count)
{
	char *tmp = dest;
	const char *s = src;

	while (count--)
		*tmp++ = *s++;
	return dest;
}

/**
 * memmove - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * Unlike memcpy(), memmove() copes with overlapping areas.
 */
void *memmove(void *dest, const void *src, size_t count)
{
	char *tmp;
	const char *s;

	if (dest <= src) {
		tmp = dest;
		s = src;
		while (count--)
			*tmp++ = *s++;
	} else {
		tmp = dest;
		tmp += count;
		s = src;
		s += count;
		while (count--)
			*--tmp = *--s;
	}
	return dest;
}

/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
#undef memcmp
int memcmp(const void *cs, const void *ct, size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}
