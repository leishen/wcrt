/*
 * WCRT  -  Win32API CRT
 *
 * stdlib atexit
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

#include <stdlib.h>

#include <atexit.h>

typedef void (__cdecl *_PVFV)(void);

// fixed size of 32 entries conforms to the C standard
_PVFV wcrt_atexit[32];

size_t wcrt_atexit_count = 0;

int atexit(_PVFV fcn)
{
    if (wcrt_atexit_count == sizeof(wcrt_atexit)/sizeof(wcrt_atexit[0]))
    {
        return -1;
    }

    wcrt_atexit[wcrt_atexit_count++] = fcn;

    return 0;
}

void wcrt_perform_atexit()
{
    // call functions registered with atexit in reverse order
    while (wcrt_atexit_count)
    {
        wcrt_atexit[--wcrt_atexit_count]();
    }
}
