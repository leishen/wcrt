/*
 * WCRT  -  Win32API CRT
 *
 * wcrt_assert
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

#include <assert.h>

void wcrt_assert(const char *exp, const char *file, int line, const char *func)
{
    char wcrt_bad_buffer[1024];

    wsprintf(wcrt_bad_buffer, "Assertion failed:\n\n   %.500s   \n\nfunction %.200s, file %.200s, line %d.", exp, func, file, line);

    MessageBox(0, wcrt_bad_buffer, "Assertion failed", MB_ICONERROR | MB_OK);

    ExitProcess(-1);
}
