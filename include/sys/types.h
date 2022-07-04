/*
 * Copyright (c) 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;     /* see this! unsigned longs at last! */
typedef unsigned short  ushort;     /* sys III compat */

#ifndef _SIZE_T
#define _SIZE_T
typedef u_int   size_t;
#endif
#ifndef __ssize_t_defined
#ifndef _SSIZE_T
typedef int     ssize_t;
#define __ssize_t_defined
#define _SSIZE_T
#endif
#endif
#ifndef _OFF_T
#define _OFF_T
typedef long    off_t;
#endif

#endif
