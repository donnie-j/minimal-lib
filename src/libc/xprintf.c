/*------------------------------------------------------------------------/
/  Universal String Handler for Console Input and Output
/-------------------------------------------------------------------------/
/
/ Copyright (C) 2021, ChaN, all right reserved.
/
/ xprintf module is an open source software. Redistribution and use of
/ xprintf module in source and binary forms, with or without modification,
/ are permitted provided that the following condition is met:
/
/ 1. Redistributions of source code must retain the above copyright notice,
/    this condition and the following disclaimer.
/
/ This software is provided by the copyright holder and contributors "AS IS"
/ and any warranties related to this software are DISCLAIMED.
/ The copyright owner or contributors be NOT LIABLE for any damages caused
/ by use of this software.
/
/-------------------------------------------------------------------------*/

#include "string.h"

#define	XF_CRLF		0	/* 1: Convert \n ==> \r\n in the output char */
#define	XF_USE_LLI	1	/* 1: Enable long long integer in size prefix ll */
#define	XF_USE_FP	0	/* 1: Enable support for floating point in type e and f */
#define XF_DPC		'.'	/* Decimal separator for floating point */

#if defined(__GNUC__) && __GNUC__ >= 10
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif

#define SZB_OUTPUT	32

#include <stdarg.h>
void (*xfunc_output)(int);	/* Pointer to the default output device */
char *_strptr;			/* Pointer to the output memory (used by xsprintf) */


#if XF_USE_FP
/*----------------------------------------------*/
/* Floating point output                        */
/*----------------------------------------------*/
#include <math.h>


static int ilog10 (double n)	/* Calculate log10(n) in integer output */
{
	int rv = 0;

	while (n >= 10) {	/* Decimate digit in right shift */
		if (n >= 100000) {
			n /= 100000; rv += 5;
		} else {
			n /= 10; rv++;
		}
	}
	while (n < 1) {		/* Decimate digit in left shift */
		if (n < 0.00001) {
			n *= 100000; rv -= 5;
		} else {
			n *= 10; rv--;
		}
	}
	return rv;
}


static double i10x (int n)	/* Calculate 10^n */
{
	double rv = 1;

	while (n > 0) {		/* Left shift */
		if (n >= 5) {
			rv *= 100000; n -= 5;
		} else {
			rv *= 10; n--;
		}
	}
	while (n < 0) {		/* Right shift */
		if (n <= -5) {
			rv /= 100000; n += 5;
		} else {
			rv /= 10; n++;
		}
	}
	return rv;
}


static void ftoa (
	char* buf,	/* Buffer to output the generated string */
	double val,	/* Real number to output */
	int prec,	/* Number of fractinal digits */
	char fmt	/* Notation */
)
{
	int d;
	int e = 0, m = 0;
	char sign = 0;
	double w;
	const char *er = 0;


	if (isnan(val)) {			/* Not a number? */
		er = "NaN";
	} else {
		if (prec < 0) prec = 6;	/* Default precision (6 fractional digits) */
		if (val < 0) {			/* Nagative value? */
			val = -val; sign = '-';
		} else {
			sign = '+';
		}
		if (isinf(val)) {		/* Infinite? */
			er = "INF";
		} else {
			if (fmt == 'f') {	/* Decimal notation? */
				val += i10x(-prec) / 2;	/* Round (nearest) */
				m = ilog10(val);
				if (m < 0) m = 0;
				if (m + prec + 3 >= SZB_OUTPUT) er = "OV";	/* Buffer overflow? */
			} else {			/* E notation */
				if (val != 0) {		/* Not a true zero? */
					val += i10x(ilog10(val) - prec) / 2;	/* Round (nearest) */
					e = ilog10(val);
					if (e > 99 || prec + 6 >= SZB_OUTPUT) {	/* Buffer overflow or E > +99? */
						er = "OV";
					} else {
						if (e < -99) e = -99;
						val /= i10x(e);	/* Normalize */
					}
				}
			}
		}
		if (!er) {	/* Not error condition */
			if (sign == '-') *buf++ = sign;	/* Add a - if negative value */
			do {				/* Put decimal number */
				w = i10x(m);				/* Snip the highest digit d */
				d = val / w; val -= d * w;
				if (m == -1) *buf++ = XF_DPC;	/* Insert a decimal separarot if get into fractional part */
				*buf++ = '0' + d;			/* Put the digit */
			} while (--m >= -prec);			/* Output all digits specified by prec */
			if (fmt != 'f') {	/* Put exponent if needed */
				*buf++ = fmt;
				if (e < 0) {
					e = -e; *buf++ = '-';
				} else {
					*buf++ = '+';
				}
				*buf++ = '0' + e / 10;
				*buf++ = '0' + e % 10;
			}
		}
	}
	if (er) {	/* Error condition? */
		if (sign) *buf++ = sign;		/* Add sign if needed */
		do *buf++ = *er++; while (*er);	/* Put error symbol */
	}
	*buf = 0;	/* Term */
}
#endif	/* XF_USE_FLOAT */


/*----------------------------------------------*/
/* Put a character                              */
/*----------------------------------------------*/

void xfputc (			/* Put a character to the specified device */
	void(*func)(int),	/* Pointer to the output function (null:_strptr) */
	int chr				/* Character to be output */
)
{
	if (XF_CRLF && chr == '\n') xfputc(func, '\r');	/* CR -> CRLF */

	if (func) {
		func(chr);		/* Write a character to the output device */
	} else if (_strptr) {
		 *_strptr++ = chr;	/* Write a character to the memory */
	}
}

void xputc (
	int chr				/* Character to be output */
)
{
	xfputc(xfunc_output, chr);	/* Output it to the default output device */
}

/*----------------------------------------------*/
/* Put a null-terminated string                 */
/*----------------------------------------------*/

void xfputs (			/* Put a string to the specified device */
	void(*func)(int),	/* Pointer to the output function */
	const char*	str		/* Pointer to the string */
)
{
	while (*str) {			/* Put the string */
		xfputc(func, *str++);
	}
}

void xputs (			/* Put a string to the default device */
	const char* str		/* Pointer to the string */
)
{
	xfputs(xfunc_output, str);
}

/*----------------------------------------------*/
/* Formatted string output                      */
/*----------------------------------------------*/
/*  xprintf("%d", 1234);			"1234"
    xprintf("%6d,%3d%%", -200, 5);	"  -200,  5%"
    xprintf("%-6u", 100);			"100   "
    xprintf("%ld", 12345678);		"12345678"
    xprintf("%llu", 0x100000000);	"4294967296"	<XF_USE_LLI>
    xprintf("%lld", -1LL);			"-1"			<XF_USE_LLI>
    xprintf("%04x", 0xA3);			"00a3"
    xprintf("%08lX", 0x123ABC);		"00123ABC"
    xprintf("%016b", 0x550F);		"0101010100001111"
    xprintf("%*d", 6, 100);			"   100"
    xprintf("%s", "String");		"String"
    xprintf("%5s", "abc");			"  abc"
    xprintf("%-5s", "abc");			"abc  "
    xprintf("%-5s", "abcdefg");		"abcdefg"
    xprintf("%-5.5s", "abcdefg");	"abcde"
    xprintf("%-.5s", "abcdefg");	"abcde"
    xprintf("%-5.5s", "abc");		"abc  "
    xprintf("%c", 'a');				"a"
    xprintf("%12f", 10.0);			"   10.000000"	<XF_USE_FP>
    xprintf("%.4E", 123.45678);		"1.2346E+02"	<XF_USE_FP>
*/

void xvfprintf (
	void(*func)(int),	/* Pointer to the output function */
	const char*	fmt,	/* Pointer to the format string */
	va_list arp			/* Pointer to arguments */
)
{
	unsigned int r, i, j, w, f;
	int n, prec;
	char str[SZB_OUTPUT], c, d, *p, pad;
#if XF_USE_LLI
	long long v;
	unsigned long long uv;
#else
	long v;
	unsigned long uv;
#endif

	for (;;) {
		c = *fmt++;					/* Get a format character */
		if (!c) break;				/* End of format? */
		if (c != '%') {				/* Pass it through if not a % sequense */
			xfputc(func, c); continue;
		}
		f = w = 0;			 		/* Clear parms */
		pad = ' '; prec = -1;
		c = *fmt++;					/* Get first char of the sequense */
		if (c == '0') {				/* Flag: left '0' padded */
			pad = '0'; c = *fmt++;
		} else {
			if (c == '-') {			/* Flag: left justified */
				f = 2; c = *fmt++;
			}
		}
		if (c == '*') {				/* Minimum width from an argument */
			n = va_arg(arp, int);
			if (n < 0) {			/* Flag: left justified */
				n = 0 - n; f = 2;
			}
			w = n; c = *fmt++;
		} else {
			while (c >= '0' && c <= '9') {	/* Minimum width */
				w = w * 10 + c - '0';
				c = *fmt++;
			}
		}
		if (c == '.') {				/* Precision */
			c = *fmt++;
			if (c == '*') {				/* Precision from an argument */
				prec = va_arg(arp, int);
				c = *fmt++;
			} else {
				prec = 0;
				while (c >= '0' && c <= '9') {
					prec = prec * 10 + c - '0';
					c = *fmt++;
				}
			}
		}
		if (c == 'l') {		/* Prefix: Size is long */
			f |= 4; c = *fmt++;
#if XF_USE_LLI
			if (c == 'l') {	/* Prefix: Size is long long */
				f |= 8; c = *fmt++;
			}
#endif
		}
		if (!c) break;				/* End of format? */
		switch (c) {				/* Type is... */
		case 'b':					/* Unsigned binary */
			r = 2; break;
		case 'o':					/* Unsigned octal */
			r = 8; break;
		case 'd':					/* Signed decimal */
		case 'u':					/* Unsigned decimal */
			r = 10; break;
		case 'x':					/* Hexdecimal (lower case) */
		case 'X':					/* Hexdecimal (upper case) */
			r = 16; break;
		case 'c':					/* A character */
			xfputc(func, (char)va_arg(arp, int)); continue;
		case 's':					/* String */
			p = va_arg(arp, char*);		/* Get a pointer argument */
			if (!p) p = "";				/* Null ptr generates a null string */
			j = strlen(p);
			if (prec >= 0 && j > (unsigned int)prec) j = prec;	/* Limited length of string body */
			for ( ; !(f & 2) && j < w; j++) xfputc(func, pad);	/* Left pads */
			while (*p && prec--) xfputc(func, *p++);/* String body */
			while (j++ < w) xfputc(func, ' ');		/* Right pads */
			continue;
#if XF_USE_FP
		case 'f':					/* Float (decimal) */
		case 'e':					/* Float (e) */
		case 'E':					/* Float (E) */
			ftoa(p = str, va_arg(arp, double), prec, c);	/* Make fp string */
			for (j = strlen(p); !(f & 2) && j < w; j++) xfputc(func, pad);	/* Left pads */
			while (*p) xfputc(func, *p++);		/* Value */
			while (j++ < w) xfputc(func, ' ');	/* Right pads */
			continue;
#endif
		default:					/* Unknown type (passthrough) */
			xfputc(func, c); continue;
		}

		/* Get an integer argument and put it in numeral */
#if XF_USE_LLI
		if (f & 8) {	/* long long argument? */
			v = (long long)va_arg(arp, long long);
		} else {
			if (f & 4) {	/* long argument? */
				v = (c == 'd') ? (long long)va_arg(arp, long) : (long long)va_arg(arp, unsigned long);
			} else {		/* int/short/char argument */
				v = (c == 'd') ? (long long)va_arg(arp, int) : (long long)va_arg(arp, unsigned int);
			}
		}
#else
		if (f & 4) {	/* long argument? */
			v = (long)va_arg(arp, long);
		} else {		/* int/short/char argument */
			v = (c == 'd') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
		}
#endif
		if (c == 'd' && v < 0) {	/* Negative value? */
			v = 0 - v; f |= 1;
		}
		i = 0; uv = v;
		do {	/* Make an integer number string */
			d = (char)(uv % r); uv /= r;
			if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
			str[i++] = d + '0';
		} while (uv != 0 && i < sizeof str);
		if (f & 1) str[i++] = '-';					/* Sign */
		for (j = i; !(f & 2) && j < w; j++) xfputc(func, pad);	/* Left pads */
		do xfputc(func, str[--i]); while (i != 0);	/* Value */
		while (j++ < w) xfputc(func, ' ');			/* Right pads */
	}
}

void xprintf (			/* Put a formatted string to the default device */
	const char*	fmt,	/* Pointer to the format string */
	...					/* Optional arguments */
)
{
	va_list arp;


	va_start(arp, fmt);
	xvfprintf(xfunc_output, fmt, arp);
	va_end(arp);
}

void xfprintf (			/* Put a formatted string to the specified device */
	void(*func)(int),	/* Pointer to the output function */
	const char*	fmt,	/* Pointer to the format string */
	...					/* Optional arguments */
)
{
	va_list arp;


	va_start(arp, fmt);
	xvfprintf(func, fmt, arp);
	va_end(arp);
}

int
sprintf (			/* Put a formatted string to the memory */
	char* buff,			/* Pointer to the output buffer */
	const char*	fmt,	/* Pointer to the format string */
	...					/* Optional arguments */
)
{
	va_list arp;


	_strptr = buff;		/* Enable destination for memory */
	va_start(arp, fmt);
	xvfprintf(0, fmt, arp);
	va_end(arp);
	*_strptr = 0;		/* Terminate output string */
	_strptr = 0;			/* Disable destination for memory */

	return 0; /* FIXME: length */
}

