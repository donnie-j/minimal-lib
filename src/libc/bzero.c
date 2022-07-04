/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

void
bzero(b, length)
	void *b;
	unsigned int length;
{
	char *p = b;
	if (length)
		do
			*p++ = '\0';
		while (--length);
}
