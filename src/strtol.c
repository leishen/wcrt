/*
 * WCRT  -  Win32API CRT
 *
 * stdlib strtol
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

long strtol(const char *s, char **endp, int base)
{
    const unsigned char *p = (unsigned char *)s;
    unsigned long res = 0;
    int negative = 0;

    // skip initial whitespaces
    while (*p && *p <= ' ') ++p;

    if (p[0] == 0)
    {
        if (endp) *endp = (char *)p;
        return 0;
    }

    // check sign
    if ((p[0] == '+') || (p[0] == '-'))
    {
        negative = p[0] == '-' ? 1 : 0;
        ++p;
    }

    // figure out base if not given
    if (base == 0)
    {
        if (p[0] != '0')
        {
            base = 10;
        } else {
            if ((p[1] == 'x') || (p[1] == 'X'))
            {
                base = 16;
                p += 2;
            } else {
                base = 8;
                ++p;
            }
        }

    } else {

        // 0x/0X is allowed for hex even when base is given
        if ((base == 16) && (p[0] == '0'))
        {
            if ((p[1] == 'x') || (p[1] == 'X'))
            {
                p += 2;
            }
        }
    }

    for ( ; p[0]; ++p)
    {
        unsigned long val = p[0];

        if (val >= 'a') val -= 'a' - 'A';

        if (val >= 'A')
        {
            val = val - 'A' + 10;
        } else {
            if (val < '0' || val > '9') break;
            val = val - '0';
        }

        if (val >= (unsigned long)base) break;

        res = res*(unsigned long)base + val;
    }

    if (endp) *endp = (char *)p;

    return negative ? -((long)res) : (long)res;
}
