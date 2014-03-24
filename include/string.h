/*
 * WCRT  -  Win32API CRT
 *
 * string.h
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

#ifndef WCRT_STRING_H_INCLUDED
#define WCRT_STRING_H_INCLUDED

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

 /* TODO:
    char *strerror(unsigned int n);
 */

    char *strcpy(char *s, const char *ct);
    char *strncpy(char *s, const char *ct, size_t n);
    char *strcat(char *s, const char *ct);
    char *strncat(char *s, const char *ct, size_t n);
    int strcmp(const char *cs, const char *ct);
    int strncmp(const char *cs, const char *ct, size_t n);
    int strcoll(const char *cs, const char *ct);
    char *strchr(const char *cs, int c);
    char *strrchr(const char *cs, int c);
    size_t strspn(const char *cs, const char *ct);
    size_t strcspn(const char *cs, const char *ct);
    char *strpbrk(const char *cs, const char *ct);
    char *strstr(const char *cs, const char *ct);
    size_t strlen(const char *cs);

    char *strtok(char *s, const char *ct);

    void *memcpy(void *s, const void *ct, size_t n);
    void *memmove(void *s, const void *ct, size_t n);
    int memcmp(const void *cs, const void *ct, size_t n);
    void *memchr(const void *cs, int c, size_t n);
    void *memset(void *s, int c, size_t n);

    /* non-standard */

    char *strlwr(char *s);
    char *strupr(char *s);
    int stricmp(const char *cs, const char *ct);

    char *strtok_r(char *s, const char *ct, char **t);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WCRT_STRING_H_INCLUDED */
