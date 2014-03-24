/*
 * WCRT  -  Win32API CRT
 *
 * wcrt_debugf
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "debugf.h"

char * __cdecl wcrt_debugf_format(const char *format, ...)
{
    char *s = (char *) malloc(4096);

    if (s != NULL)
    {
        va_list ap;

        va_start(ap, format);

#ifdef WCRT_USE_C_PRINTF
        vsnprintf(s, 4095, format, ap);
#else
        vsprintf(s, format, ap);
#endif

        va_end(ap);
    }

    return s;
}

void __cdecl wcrt_debugf_out(const char *file, int line, const char *func, char *s)
{
    char buffer[4096];
    char *ps = s;

    if (ps == NULL) ps = "WCRT debugf internal error: out of memory.";

    sprintf(buffer, "%.500s(%d): %.500s: %.3000s\r\n", file, line, func, ps);

    if (s != NULL) free(s);

    OutputDebugString(buffer);
}
