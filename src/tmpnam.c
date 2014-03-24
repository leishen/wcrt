/*
 * WCRT  -  Win32API CRT
 *
 * stdio tmpnam
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

char *tmpnam(char *s)
{
    static char wcrt_bad_buffer[MAX_PATH];
    static unsigned short uUnique = 0;

    if (!s) s = wcrt_bad_buffer;

    ++uUnique;

    if (uUnique == 0) ++uUnique;

    if (!GetTempFileName(".", "wcx", uUnique, s)) return NULL;

    return s;
}
