/*
 * WCRT  -  Win32API CRT
 *
 * wcrt_printf
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
#include <ctype.h>
#include <stdarg.h>

#include "_printf.h"

// These are the internal functions used as the base for the
// *printf family of functions.

// TODO:
//
//   - float conversions are missing
//   - no wide character support

// put a single character to stream
static void wcrt_printf_put(WCRT_PRINTF_DATA *wpd, int c)
{
    // discard characters beyond limit, but still count them
    if (wpd->num_written++ >= wpd->limit) return;

    if (wpd->is_stream)
    {
        fputc(c, (FILE *) wpd->dst);

    } else {

        *((unsigned char *) wpd->dst)++ = c;
    }
}

// shorthand
#define PUT(c) wcrt_printf_put(wpd, (c))

typedef struct {
    unsigned int  left;
    unsigned int  sign;
    unsigned int  blank;
    unsigned char fill;
    unsigned int  alt;
    unsigned int  width;
    unsigned int  prec;
    unsigned char lmod;
} wcrt_outf_state;

//
// output a 32-bit integer value in octal, decimal or hex
//

static void wcrt_printf_int(
    WCRT_PRINTF_DATA *wpd,
    wcrt_outf_state *os, // state flags
    unsigned int base,   // base to output number in
    int v,               // the number
    unsigned char f,     // the conversion specifier (type)
    int is_u)            // non-zero if v represents an unsigned value
{
    static const char nib2x[] = "0123456789abcdef";
    static const char nib2X[] = "0123456789ABCDEF";

    unsigned char rev[32];

    unsigned int len = 0;
    unsigned int jlen;
    unsigned int wlen;
    unsigned int i;
    unsigned char c = 0;
    const char *nib2 = (f == 'x') ? nib2x : nib2X;

    // set precision to default if not specified
    if (os->prec == 0xffffffff) os->prec = 1; else os->fill = ' ';

    // correct value for short modifier
    if (os->lmod == 'h')
    {
        if (is_u) v = v & 0xffff; else v = (int)(short)v;
    }

    // put string into rev[] in reverse order

    {
        unsigned int vv;

        if (is_u) vv = v; else vv = (v < 0) ? -v : v;

        if ((vv == 0) && os->prec) rev[len++] = '0';

        while (vv > 0)
        {
            rev[len++] = nib2[vv % base];
            vv /= base;
        }
    }

    // sign character

    if (!is_u)
    {
        if (v >= 0)
        {
            if (os->sign) c = '+';
            else if (os->blank) c = ' ';

        } else {

            c = '-';
        }
    }

    // compute precision width

    wlen = (len > os->prec) ? len : os->prec;

    // compute justification width

    jlen = (len > os->prec) ? len : os->prec;

    if (c) ++jlen; // sign character

    // handle special case of octal output in alt format where it is
    // not required to add an extra leading zero
    if (os->alt && (f == 'o'))
    {
        // if we will be adding rigth-justification with zero
        if (!os->left && (os->fill == '0') && (jlen < os->width)) os->alt = 0;
        // if we will be adding zero fill up to precision
        if (wlen > len) os->alt = 0;
    }

    if (os->alt)
    {
        switch (base)
        {
        case 16: ++jlen; // will get 0x or 0X prepended (2 chars)
        case 8:  ++jlen; // will get 0 prepended (1 char)
        }
    }

    // output string

    // handle right-justification with space
    if (!os->left && (os->fill == ' ') && (jlen < os->width))
    {
        for (i = os->width - jlen; i > 0; --i) PUT(' ');
    }

    // sign character
    if (c) PUT(c);

    // alternate prefix
    if (os->alt)
    {
        switch (base)
        {
        case 16: PUT('0'); PUT(f); break;
        case 8:  PUT('0'); break;
        }
    }

    // handle right-justification with zero
    if (!os->left && (os->fill == '0') && (jlen < os->width))
    {
        for (i = os->width - jlen; i > 0; --i) PUT('0');
    }

    // zero-fill up to precision
    for (i = wlen - len; i > 0; --i) PUT('0');

    // the number itself
    for (i = len; i > 0; --i)
    {
        PUT(rev[i - 1]);
    }

    // handle left-justification
    if (os->left && (jlen < os->width))
    {
        for (i = os->width - jlen; i > 0; --i) PUT(os->fill);
    }
}

//
// output a 64-bit floating-point value
//

static void wcrt_printf_float(
    WCRT_PRINTF_DATA *wpd,
    wcrt_outf_state *os, // state flags
    unsigned int v[2],   // the number
    unsigned char f)     // the conversion specifier (type)
{
    unsigned int len = 0;
    unsigned char c = 0;
    int sign = (v[0] & 0x80000000) ? -1 : 1;
    int exp = ((v[0] >> 20) & 0x07ff) - 0x03ff;
    unsigned int manth = v[0] & 0x000fffff;

    // set precision to default if not specified
    if (os->prec == 0xffffffff) os->prec = 6;

    // clear out sign and exponent, so only mantissa reamins in v[]
    v[0] &= 0x000fffff;

    if (sign > 0)
    {
        if (os->sign) c = '+';
        else if (os->blank) c = ' ';

    } else {

        c = '-';
    }

    if (c) PUT(c);

    PUT('e');

    wcrt_printf_int(wpd, os, 10, exp, 'd', 0);
}

int wcrt_printf(const char *format,
                WCRT_PRINTF_DATA *wpd,
                va_list arg)
{
    static const char null[]  = "(null)";
    const unsigned char *f = (const unsigned char *)format;

    if (wpd->limit == 0) return 0;

    while (*f)
    {
        wcrt_outf_state os = { 0, 0, 0, ' ', 0, 0, 0xffffffff, 0 };

        // copy up to next %

        while (*f && (*f != '%')) PUT(*f++);

        // parse modifiers

        while (*f)
        {
            ++f;
            if (*f == '-') { os.left  = 1; os.fill = ' ';  continue; }
            if (*f == '+') { os.sign  = 1; os.blank = 0;   continue; }
            if (*f == ' ') { if (!os.sign) os.blank = 1;   continue; }
            if (*f == '0') { if (!os.left) os.fill  = '0'; continue; }
            if (*f == '#') { os.alt   = 1;                 continue; }
            break;
        }

        // parse width if present

        while (isdigit(*f))
        {
            os.width = 10*os.width + (*f - '0');
            ++f;
        }

        if (*f == '*')
        {
            int v = va_arg(arg, int);
            if (v < 0) { v = -v; os.left = 1; os.fill = ' '; }
            os.width = v;
            ++f;
        }

        // parse precision if present

        if (*f == '.')
        {
            ++f;

            os.prec = 0;

            while (isdigit(*f))
            {
                os.prec = 10*os.prec + (*f - '0');
                ++f;
            }

            if (*f == '*')
            {
                int v = va_arg(arg, int);
                if (v >= 0) os.prec = v;
                ++f;
            }
        }

        // parse length modifier if present

        if ((*f == 'h') || (*f == 'l') || (*f == 'L'))
        {
            os.lmod = *f++;
        }

        // escape if we reached end of format string

        if (*f == 0) break;

        // parse convertion

        switch (*f)
        {
        case 'b': /* binary output, non-standard */
            {
                unsigned int v = va_arg(arg, unsigned int);
                wcrt_printf_int(wpd, &os, 2, v, 'b', 1);
            }
            break;
        case 'd':
        case 'i':
            {
                int v = va_arg(arg, int);
                wcrt_printf_int(wpd, &os, 10, v, 'd', 0);
            }
            break;
        case 'o':
            {
                unsigned int v = va_arg(arg, unsigned int);
                wcrt_printf_int(wpd, &os, 8, v, 'o', 1);
            }
            break;
        case 'u':
            {
                unsigned int v = va_arg(arg, unsigned int);
                wcrt_printf_int(wpd, &os, 10, v, 'u', 1);
            }
            break;
        case 'x':
        case 'X':
            {
                unsigned int v = va_arg(arg, unsigned int);
                wcrt_printf_int(wpd, &os, 16, v, *f, 1);
            }
            break;
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
        case 'a':
        case 'A':
            {
                unsigned int v[2];

                v[1] = va_arg(arg, unsigned int);
                v[0] = va_arg(arg, unsigned int);

                wcrt_printf_float(wpd, &os, v, *f);
            }
            break;
        case 'c':
            {
                int v = va_arg(arg, int);
                PUT((unsigned char) v);
            }
            break;
        case 's':
            {
                const char *v = va_arg(arg, char *);
                unsigned int len;
                unsigned int i;

                // set precision to default if not specified
                if (os.prec == 0xffffffff) os.prec = 0;

                if (v == NULL) v = null;

                len = strlen(v);

                // compute justification width
                if (os.prec && (len > os.prec)) len = os.prec;

                // handle right-justification
                if (!os.left && (len < os.width))
                {
                    for (i = os.width - len; i > 0; --i) PUT(os.fill);
                }

                // the string itself
                for (i = 0; i < len; ++i)
                {
                    PUT(*v++);
                }

                // handle left-justification
                if (os.left && (len < os.width))
                {
                    for (i = os.width - len; i > 0; --i) PUT(os.fill);
                }
            }
            break;
        case 'p':
            {
                unsigned int v = va_arg(arg, unsigned int);
                wcrt_outf_state tmp = { 0, 0, 0, '0', 0, 8, 8, 0 };
                wcrt_printf_int(wpd, &tmp, 16, v, 'X', 1);
            }
            break;
        case 'n':
            if (os.lmod == 'h')
            {
                short *v = va_arg(arg, short *);
                if (v) *v = wpd->num_written;
            } else {
                int *v = va_arg(arg, int *);
                if (v) *v = wpd->num_written;
            }
            break;
        case '%':
            PUT('%');
            break;
        }

        ++f;
    }

    return wpd->num_written;
}
