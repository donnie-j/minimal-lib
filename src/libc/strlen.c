/*
 * Returns the number of
 * non-NULL bytes in string argument.
 */
#include <string.h>

unsigned int
strlen(s)
	const char *s;
{
	int n;

	n = 0;
	while (*s++)
		n++;
	return(n);
}
