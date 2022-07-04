/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

void
bcopy(src, dst, length)
	const void *src;
	void *dst;
	unsigned int length;
{
	const char *s = src;
	char *d = dst;
	if (length && s != d) {
		if (d < s)
			do
				*d++ = *s++;
			while (--length);
		else {			/* copy backwards */
			s += length;
			d += length;
			do
				*--d = *--s;
			while (--length);
		}
	}
}
