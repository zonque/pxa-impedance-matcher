#include "types.h"
#include "string.h"

int hexlut[1 + 'F' - '0'] = {
	0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, /* 0 - 9 */
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0xa, 0xb, 0xc, 0xd, 0xe, 0xf                      /* A - F */
};

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
