/*-
 * Copyright (c) 1991, 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Modified for 2.11BSD by removing prototypes.  To save time and space
 * functions not returning 'int' and functions not present in the system
 * are not listed.
*/

#ifndef _UNISTD_H_
#define _UNISTD_H_

#include <sys/types.h>

#define STDIN_FILENO    0       /* standard input file descriptor */
#define STDOUT_FILENO   1       /* standard output file descriptor */
#define STDERR_FILENO   2       /* standard error file descriptor */

#ifndef NULL
#define NULL            0       /* null pointer constant */
#endif

/* Values for the second argument to access.
   These may be OR'd together.  */
#define R_OK            4       /* Test for read permission.  */
#define W_OK            2       /* Test for write permission.  */
#define X_OK            1       /* Test for execute permission.  */
#define F_OK            0       /* Test for existence.  */

void    _exit (int);
off_t   lseek();
ssize_t read();
unsigned int    sleep();
ssize_t write (int fd, const void *buf, size_t count);

int     brk (void *addr);
void    *sbrk (int incr);
void    usleep();
int     close (int fd);
int     unlink (const char *pathname);
int     link (const char *oldpath, const char *newpath);
ssize_t readlink (const char *path, char *buf, size_t bufsiz);
int     chdir (const char *path);
int     rmdir (const char *pathname);

struct stat;
int     stat (const char *path, struct stat *buf);
int     fstat (int fd, struct stat *buf);
int     lstat (const char *path, struct stat *buf);


#ifndef _VA_LIST_
#  define va_list   __builtin_va_list   /* For Gnu C */
#endif

#ifndef _VA_LIST_
# undef va_list
#endif
#endif /* !_UNISTD_H_ */
