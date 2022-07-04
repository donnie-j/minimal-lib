/*
 * ISO C Standard:  7.15  Variable arguments  <stdarg.h>
 */
#ifndef _STDARG_H
#define _STDARG_H

/*
 * Define va_start, va_arg, va_end, va_copy.
 */
# define va_start(ap, last)     __builtin_va_start((ap), last)
# define va_arg(ap, type)       __builtin_va_arg((ap), type)
# define va_end(ap)             __builtin_va_end((ap))
# define va_copy(dest, src)     __builtin_va_copy((dest), (src))

/*
 * Define va_list.
 */
#ifndef _VA_LIST_T
# define _VA_LIST_T
    typedef __builtin_va_list va_list;
#endif

/*
 * Define __gnuc_va_list.
 */
#if !defined(__GNUC_VA_LIST)
# define __GNUC_VA_LIST
    typedef __builtin_va_list __gnuc_va_list;
#endif

#endif /* not _STDARG_H */
