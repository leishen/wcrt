/*
 * WCRT  -  Win32API CRT
 *
 * string strtok
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

#include <string.h>

char *strtok(char *s, const char *ct)
{
    static char *t;
    const char *p;
    char *beg;
    char *end;

    if (s) t = s;

    beg = t;

    // find first char in rest, which is not in ct
    for ( ; *beg; ++beg)
    {
       p = ct;

       while (*p && (*p != *beg)) ++p;

       if (*p == '\0') break;
    }

    end = beg;

    // find first char in rest, which is in ct
    // and zero terminate token
    for (p = ct; *end; )
    {
        if (*end == *p)
        {
            *end++ = '\0';
            break;

        } else {

            ++p;
            
            if (*p == '\0')
            {
                p = ct;
                ++end;
            }
        }
    }

    t = end;

    return beg == end ? 0 : beg;
}
