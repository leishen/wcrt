/*
 * WCRT  -  Win32API CRT
 *
 * time asctime
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

char *asctime(const struct tm *tp)
{
    static const char day_names[] = "Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat";
    static const char month_names[] = "Jan\0Feb\0Mar\0Apr\0May\0Jun\0Jul\0Aug\0Sep\0Oct\0Nov\0Dec";

    static char wcrt_bad_buffer[72];

    wsprintf(
        wcrt_bad_buffer,
        "%.3s %.3s %d %.2d:%.2d:%.2d %d\n",
        &day_names[4*tp->tm_wday],
        &month_names[4*tp->tm_mon],
        tp->tm_mday,
        tp->tm_hour, tp->tm_min, tp->tm_sec,
        tp->tm_year + 1900);

    return wcrt_bad_buffer;
}
