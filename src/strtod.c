/*
 * WCRT  -  Win32API CRT
 *
 * stdlib strtod
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

double strtod(const char *s, char **endp)
{
    const unsigned char *p = (unsigned char *)s;
    double res = 0;
    int negative = 0;
    unsigned int val;

    // skip initial whitespaces
    while (*p && *p <= ' ') ++p;

    if (p[0] == 0)
    {
        if (endp) *endp = (char *)p;
        return 0.0;
    }

    // check sign
    if (p[0] == '+' || p[0] == '-')
    {
        negative = p[0] == '-' ? 1 : 0;
        ++p;
    }

    // get non-fractional part
    for ( ; (val = p[0] - '0') < 10; ++p) res = 10.0*res + val;

    // get fractional part
    if (p[0] == '.')
    {
        double factor = 0.1;

        for (++p; (val = p[0] - '0') < 10; ++p)
        {
            res += val*factor;
            factor *= 0.1;
        }
    }

    // get exponent
    if (p[0] == 'e' || p[0] == 'E')
    {
        int exponent = 0;
        double factor = 10.0;

        ++p;

        // check sign
        if (p[0] == '+' || p[0] == '-')
        {
            if (p[0] == '-') factor = 0.1;
            ++p;
        }

        // get exponent value
        for ( ; (val = p[0] - '0') < 10; ++p) exponent = 10*exponent + val;

        // perform exponentiation
        for ( ; exponent; exponent >>= 1)
        {
            if (exponent & 0x01) res *= factor;
            // TODO: possible overflow?
            factor *= factor;
        }
    }

    if (endp) *endp = (char *)p;

    return negative ? -res : res;
}
