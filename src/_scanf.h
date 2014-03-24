/*
 * WCRT  -  Win32API CRT
 *
 * wcrt_scanf
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

#ifndef WCRT__SCANF_H_INCLUDED
#define WCRT__SCANF_H_INCLUDED

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        void *src;
        int is_stream;
        unsigned int num_read;
    } WCRT_SCANF_DATA;

    int wcrt_scanf(const char *format,
                   WCRT_SCANF_DATA *wsd,
                   va_list arg);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WCRT__SCANF_H_INCLUDED */
