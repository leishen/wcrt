/*
 * WCRT  -  Win32API CRT
 *
 * stdlib.h
 *
 * Copyright (c) 2003-2004 by Joergen Ibsen / Jibz
 * All Rights Reserved
 *
 * http://www.ibsensoftware.com/
 *
 * This software is provided 'as-is', without any express
 * or implied warranty.  In no event will the authors be
 * held liable for any damages arising from the use of
 * this software.
 *
 * Permission is granted to anyone to use this software
 * for any purpose, including commercial applications,
 * and to alter it and redistribute it freely, subject to
 * the following restrictions:
 *
 * 1. The origin of this software must not be
 *    misrepresented; you must not claim that you
 *    wrote the original software. If you use this
 *    software in a product, an acknowledgment in
 *    the product documentation would be appreciated
 *    but is not required.
 *
 * 2. Altered source versions must be plainly marked
 *    as such, and must not be misrepresented as
 *    being the original software.
 *
 * 3. This notice may not be removed or altered from
 *    any source distribution.
 */

#ifndef WCRT_STDLIB_H_INCLUDED
#define WCRT_STDLIB_H_INCLUDED

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

 /* TODO:
    int system(const char *s);

 */

    typedef struct {
        int quot;
        int rem;
    } div_t;

    typedef struct {
        long quot;
        long rem;
    } ldiv_t;

    #define EXIT_FAILURE 1
    #define EXIT_SUCCESS 0

    #define RAND_MAX 0x7fff

    double atof(const char *s);
    int atoi(const char *s);
    long atol(const char *s);

    double strtod(const char *s, char **endp);
    long strtol(const char *s, char **endp, int base);
    unsigned long strtoul(const char *s, char **endp, int base);

    int rand(void);
    void srand(unsigned int seed);

    void *calloc(size_t nobj, size_t size);
    void *malloc(size_t size);
    void *realloc(void *p, size_t size);
    void free(void *p);

    void abort(void);
    void exit(int status);
    int atexit(void (*fcn)(void));

    char *getenv(const char *name);

    void *bsearch(const void *key, const void *base,
                  size_t n, size_t size,
                  int (*cmp)(const void *keyval, const void *datum));

    void qsort(void *base, size_t n, size_t size,
               int (*cmp)(const void *, const void *));

    int abs(int n);
    long labs(long n);

    div_t div(int num, int denom);
    ldiv_t ldiv(long num, long denom);

    /* non-standard */

    int rand_r(unsigned int *seed);
    int rand_s(void);

    unsigned int _rotl(unsigned int val, int shift);
    unsigned int _rotr(unsigned int val, int shift);
    unsigned long _lrotl(unsigned long val, int shift);
    unsigned long _lrotr(unsigned long val, int shift);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WCRT_STDLIB_H_INCLUDED */
