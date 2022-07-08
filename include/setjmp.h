#ifndef	_SETJMP_H
#define	_SETJMP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __jmp_buf {
	unsigned long regs[9];
} jmp_buf[1];

#define __setjmp_attr __attribute__((returns_twice))
#define __longjmp_attr

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
int _setjmp (jmp_buf) __setjmp_attr;
void _longjmp (jmp_buf, int) __longjmp_attr;
#endif

int setjmp (jmp_buf) __setjmp_attr;
void longjmp (jmp_buf, int) __longjmp_attr;

#define setjmp setjmp

#undef __setjmp_attr
#undef __longjmp_attr

#ifdef __cplusplus
}
#endif

#endif
