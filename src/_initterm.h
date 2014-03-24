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

#ifndef WCRT__INITTERM_H_INCLUDED
#define WCRT__INITTERM_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (__cdecl *_PVFV)(void);

    typedef int (__cdecl *_PIFV)(void);

    /* C initialization functions */
    extern _PVFV __xc_a[];
    extern _PVFV __xc_z[];

    /* C++ initialization functions */
    extern _PIFV __xi_a[];
    extern _PIFV __xi_z[];

    /* C pre-termination functions */
    extern _PVFV __xp_a[];
    extern _PVFV __xp_z[];

    /* C termination functions */
    extern _PVFV __xt_a[];
    extern _PVFV __xt_z[];

    int _initterm(void *pfbegin, void *pfend, int is_PIFV);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WCRT__INITTERM_H_INCLUDED */
