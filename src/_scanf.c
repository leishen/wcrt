/*
 * WCRT  -  Win32API CRT
 *
 * wcrt_scanf
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

#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include "_scanf.h"

// These are the internal functions used as the base for the
// *scanf family of functions.

// TODO:
//
//   - float conversion is imprecise
//   - 'c', 's' and '[' conversions with 'l' prefix
//     (wchar_t input) missing
//   - float width checking not tested

// get next character from stream
static int wcrt_scanf_get(WCRT_SCANF_DATA *wsd)
{
    ++wsd->num_read;

    if (wsd->is_stream)
    {
        return fgetc((FILE *) wsd->src);

    } else {

        int res = *(unsigned char *)wsd->src;

        if (res)
        {
            ++((char *) wsd->src);

            return res;

        } else {

            return EOF;
        }
    }
}

// put a single character back into stream
static void wcrt_scanf_unget(WCRT_SCANF_DATA *wsd, int c)
{
    --wsd->num_read;

    if (wsd->is_stream)
    {
        ungetc(c, (FILE *) wsd->src);

    } else {

        if (c != EOF) --((char *) wsd->src);
    }
}

// shorthands
#define GET()    wcrt_scanf_get(wsd)
#define UNGET(c) wcrt_scanf_unget(wsd, (c))

// read a floating point number
int wcrt_scanf_float(WCRT_SCANF_DATA *wsd,
                     size_t width,
                     int mod,
                     void *v)
{
    double res = 0;
    int negative = 0;
    int c;
    int num = 0;

    c = GET();

    // check sign
    if (c == '+' || c == '-')
    {
        negative = (c == '-') ? 1 : 0;

        c = GET();

        --width;
    }

    if (c == EOF) { UNGET(c); return EOF; }

    if (width-- == 0) { UNGET(c); return 0; }

    // get non-fractional part
    for ( ; width && isdigit(c); c = GET(), --width)
    {
        res = 10.0*res + (c - '0');

        ++num;
    }

    // get fractional part
    if (c == '.' && width)
    {
        double frac = 0;
        double dist = 1.0L;

        c = GET();

        --width;

        for ( ; width && isdigit(c); c = GET(), --width)
        {
            frac = 10.0*frac + (c - '0');
            dist *= 10.0;

            ++num;
        }

        res += frac / dist;
    }

    // get exponent
    if ((c == 'e' || c == 'E') && width)
    {
        int exponent = 0;
        double factor = 10.0;

        c = GET();

        if (--width == 0) { UNGET(c); return 0; }

        // check sign
        if (c == '+' || c == '-')
        {
            if (c == '-') factor = 0.1;
            c = GET();

            --width;
        }

        // get exponent value
        for ( ; width && isdigit(c); c = GET(), --width)
        {
            exponent = 10*exponent + (c - '0');
        }

        // perform exponentiation
        for ( ; exponent; exponent >>= 1)
        {
            if (exponent & 0x01) res *= factor;
            // TODO: possible overflow?
            factor *= factor;
        }
    }

    UNGET(c);

    if (num == 0) return 0;

    if (negative) res = -res;

    if (v)
    {
        switch (mod)
        {
        case 3:  *(double *)v       = (double)res;      break;
        case 8:  *(long double *)v  = (long double)res; break;
        default: *(float *)v        = (float)res;       break;
        }
    }

    return 1;
}

// read an integer number (in the specified base)
int wcrt_scanf_int(WCRT_SCANF_DATA *wsd,
                   unsigned int base,
                   size_t width,
                   int mod,
                   void *v)
{
    unsigned int val = 0;
    int negative = 0;
    int c;
    int num = 0;

    c = GET();

    // parse sign if present
    if (c == '-' || c == '+')
    {
        negative = (c == '-') ? 1 : 0;

        c = GET();

        width--;
    }

    if (c == EOF) { UNGET(c); return EOF; }

    while (width-- && c != EOF)
    {
        unsigned int digit = (unsigned char)c;

        if (digit >= 'a') digit -= 'a' - 'A';

        if (digit >= 'A')
        {
            digit = digit - 'A' + 10;
        } else {
            if (digit < '0' || digit > '9') break;
            digit = digit - '0';
        }

        if (digit >= base) break;

        val = base*val + digit;

        ++num;

        c = GET();
    }

    UNGET(c);

    if (num == 0) return 0;

    if (negative) val = -(int)val;

    if (v)
    {
        switch (mod)
        {
        case 1:  *(short *)v   = val; break;
        case 2:  *(char *)v    = val; break;
        case 4:  *(__int64 *)v = val; break;
        default: *(int *)v     = val; break;
        }
    }

    return 1;
}

int wcrt_scanf(const char *format,
               WCRT_SCANF_DATA *wsd,
               va_list arg)
{
    const unsigned char *f = (const unsigned char *)format;
    int res = 0;

    while (*f)
    {
        // if white-space
        if (isspace(*f))
        {
            // skip white-spaces
            do ++f; while (isspace(*f));

            // skip white-spaces in input
            {
                int c;

                do c = GET(); while (isspace(c));

                UNGET(c);
            }

            continue;
        }

        // if conversion
        if (f[0] == '%' && f[1] != '%')
        {
            size_t width = 0;
            int mod = 0;
            void *v = 0;

            ++f;

            // parse assignment supressor
            if (*f == '*')
            {
                ++f;
            } else {
                v = va_arg(arg, void *);
            }

            // parse maximum field width
            while (isdigit(*f))
            {
                width = 10*width + (*f - '0');
                ++f;
            }

            if (width == 0) width = (size_t) -1;

            // parse length modifier
            switch (*f++)
            {
            case 'h':
                mod = 1;
                if (*f == 'h')
                {
                    ++f;
                    mod = 2;
                }
                break;
            case 'l':
                mod = 3;
                if (*f == 'l')
                {
                    ++f;
                    mod = 4;
                }
                break;
            case 'j': mod = 5; break;
            case 'z': mod = 6; break;
            case 't': mod = 7; break;
            case 'L': mod = 8; break;
            case 'I': // VC++ extension 'I64' instead of 'll' .. *sigh*
                if (f[0] == '6' && f[1] == '4')
                {
                    f += 2;
                    mod = 4;
                    break;
                }
            default:
                --f;
                break;
            }

            // skip input white-space characters if appropriate
            if (*f != '[' && *f != 'c' && *f != 'n')
            {
                int c;

                do c = GET(); while (isspace(c));

                UNGET(c);

                if (res == 0 && c == EOF) return EOF;
            }

            if (*f != 'n')
            {
                int c = GET();

                UNGET(c);

                if (c == EOF) return (res == 0) ? EOF : res;
            }

            // parse conversion specifier
            switch (*f++)
            {
            case 'd':
            case 'i':
            case 'u':
                {
                    int tmp = wcrt_scanf_int(wsd, 10, width, mod, v);

                    if (tmp == EOF) return (res == 0) ? EOF : res;

                    if (tmp == 0) return res;
                }
                break;

            case 'o':
                {
                    int tmp = wcrt_scanf_int(wsd, 8, width, mod, v);

                    if (tmp == EOF) return (res == 0) ? EOF : res;

                    if (tmp == 0) return res;
                }
                break;

            case 'x':
            case 'X':
                {
                    int tmp = wcrt_scanf_int(wsd, 16, width, mod, v);

                    if (tmp == EOF) return (res == 0) ? EOF : res;

                    if (tmp == 0) return res;
                }
                break;

            case 'c':
                {
                    int c;

                    if (width == (size_t) -1) width = 1;

                    while (width--)
                    {
                        c = GET();
                        if (c == EOF)
                        {
                            UNGET(c);
                            return res == 0 ? EOF : res;
                        }
                        if (v) *((char *)v)++ = (char)c;
                    }
                }
                break;

            case 's':
                {
                    int c;

                    while (width--)
                    {
                        c = GET();

                        if (c == EOF || isspace(c))
                        {
                            UNGET(c);
                            break;
                        }

                        if (v) *((char *)v)++ = (char)c;
                    }

                    if (v) *((char *)v) = '\0';
                }
                break;

            case 'a':
            case 'e':
            case 'f':
            case 'g':
            case 'A':
            case 'E':
            case 'F':
            case 'G':
                {
                    int tmp;

                    tmp = wcrt_scanf_float(wsd, width, mod, v);

                    if (tmp == EOF) return (res == 0) ? EOF : res;

                    if (tmp == 0) return res;
                }
                break;

            case 'p':
                {
                    int tmp = wcrt_scanf_int(wsd, 16, 8, 0, v);

                    if (tmp == EOF) return (res == 0) ? EOF : res;

                    if (tmp == 0) return res;
                }
                break;

            case 'n':
                if (v)
                {
                    --res;

                    switch (mod)
                    {
                    case 1:  *(short *)v   = wsd->num_read; break;
                    case 2:  *(char *)v    = wsd->num_read; break;
                    case 4:  *(__int64 *)v = wsd->num_read; break;
                    default: *(int *)v     = wsd->num_read; break;
                    }
                }
                break;

            case '[':
                {
                    int inv = 0;
                    int c;
                    const unsigned char *fstart = f;
                    int num = 0;

                    // handle inversion
                    if (*f == '^')
                    {
                        inv = 1;
                        ++fstart;
                        ++f;
                    }

                    // handle ']'
                    if (*f == ']') ++f;

                    // find end
                    while (*f && *f != ']') ++f;

                    for (c = GET(); c != EOF; c = GET())
                    {
                        const unsigned char *ff;

                        // find a match
                        for (ff = fstart; ff < f; ++ff)
                        {
                            if (c == *ff) break;
                        }

                        // break depending on match status
                        if (inv)
                        {
                            if (ff != f) break;
                        } else {
                            if (ff == f) break;
                        }

                        // copy a character
                        if (v) *((char *)v)++ = (char)c;

                        ++num;
                    }

                    UNGET(c);

                    if (*f == ']') ++f;

                    if (num == 0) return res;

                    if (v) *((char *)v) = '\0';
                }
                break;

            default:
                return res;
            }

            if (v) ++res;

            continue;
        }

        // else it must match the next input character
        {
            int c = GET();

            if (*f != c)
            {
                UNGET(c);
                if (res == 0 && c == EOF) return EOF;
                return res;
            }

            ++f;
        }
    }

    return res;
}
