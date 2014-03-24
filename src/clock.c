/*
 * WCRT  -  Win32API CRT
 *
 * time clock
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

#include <time.h>

clock_t wcrt_start_clock;

clock_t clock(void)
{
    return GetTickCount() - wcrt_start_clock;
}

/* initialisation */

int __cdecl __init_time(void);

#pragma data_seg(".CRT$XIC")
static int (__cdecl *pinit)(void) = __init_time;

#pragma data_seg()

int __cdecl __init_time(void)
{
    // get time for start of process
    // (clock() returns the running time)
    wcrt_start_clock = GetTickCount();

    return 0;
}
