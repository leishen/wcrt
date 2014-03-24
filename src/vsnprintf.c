/*
 * WCRT  -  Win32API CRT
 *
 * stdio vsnprintf
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

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <stdio.h>
#include <stdarg.h>

#include "_printf.h"

int vsnprintf(char *s, size_t n, const char *format, va_list arg)
{
    int res;
    WCRT_PRINTF_DATA wpd = { 0, 0, 0, 0xffffffff };

    wpd.dst   = s;
    wpd.limit = n;

    res = wcrt_printf(format, &wpd, arg);

    if (n) s[((size_t) res >= n) ? n-1 : res] = '\0';

    return res;
}
