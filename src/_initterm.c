/*
 * WCRT  -  Win32API CRT
 *
 * _initterm
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

#include <_initterm.h>

/*
 * From the CRT sources and an article by Matt Pietrek, we get
 * the following information:
 *
 * The compiler emits a table of pointers to static constructors
 * to a special section ".CRT$XCU". The linker combines sections
 * with identical names, and as a special feature, it combines
 * sections with a '$' in their name, so they are ordered
 * asciibetically by the part after the '$'.
 *
 * So by defining a variable in a section called ".CRT$XCA" and
 * another in a section called ".CRT$XCZ", we get pointers to
 * the beginning and end of the ".CRT$XCU" section.
 */

#pragma data_seg(".CRT$XCA")
_PVFV __xc_a[] = { 0 };

#pragma data_seg(".CRT$XCZ")
_PVFV __xc_z[] = { 0 };

#pragma data_seg(".CRT$XIA")
_PIFV __xi_a[] = { 0 };

#pragma data_seg(".CRT$XIZ")
_PIFV __xi_z[] = { 0 };

#pragma data_seg(".CRT$XPA")
_PVFV __xp_a[] = { 0 };

#pragma data_seg(".CRT$XPZ")
_PVFV __xp_z[] = { 0 };

#pragma data_seg(".CRT$XTA")
_PVFV __xt_a[] = { 0 };

#pragma data_seg(".CRT$XTZ")
_PVFV __xt_z[] = { 0 };

#pragma data_seg()

#pragma comment(linker, "/merge:.CRT=.data")

int _initterm(void *pfbegin, void *pfend, int is_PIFV)
{
    _PIFV *pifbegin = (_PIFV *) pfbegin;
    _PIFV *pifend   = (_PIFV *) pfend;

    while (pifbegin < pifend)
    {
        if (*pifbegin)
        {
            int res = (**pifbegin)();
            if (is_PIFV && res) return res;
        }

        ++pifbegin;
    }

    return 0;
}
