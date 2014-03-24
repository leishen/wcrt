/*
 * WCRT  -  Win32API CRT
 *
 * time.h
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

#ifndef WCRT_TIME_H_INCLUDED
#define WCRT_TIME_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

 /* TODO:
    time_t time(time_t *tp);

    double difftime(time_t time2, time_t time1);

    time_t mktime(struct tm *tp);

    char *ctime(const time_t *tp);
    struct tm *gmtime(const time_t *tp);
    struct tm *localtime(const time_t *tp);
    size_t strftime(char *s, size_t smax,
                    const char *fmt, const struct tm *tp));
 */

    typedef unsigned int clock_t;

    typedef unsigned int time_t;

    #define CLOCKS_PER_SEC 1000

    struct tm {
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
    };

    clock_t clock(void);

    char *asctime(const struct tm *tp);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WCRT_TIME_H_INCLUDED */
