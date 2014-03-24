/*
 * WCRT  -  Win32API CRT
 *
 * crt0dll
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

#include <stdlib.h>

#include <atexit.h>
#include <_initterm.h>

#ifdef _MSC_VER
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#endif

/*
 * According to the C standard, the compiler sets _FPinit to zero,
 * unless it is defined (with an initialisation) to a different value
 * in another place.
 *
 * I.e. if _fpinit is linked in, because _fltused is referenced,
 * then this pointer will point to wcrt_fpinit.
 */

int (__cdecl *_FPinit)();

extern BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID);

BOOL WINAPI _DllMainCRTStartup(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    BOOL res;

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        // check and init fpu if required,
        // exit if required, but not available
        if (_FPinit && (_FPinit() == 0)) ExitProcess(-2);

        // call c initialisers
        if (_initterm(__xi_a, __xi_z, 1)) ExitProcess(-3);

        // call c++ initialisers
        _initterm(__xc_a, __xc_z, 0);
    }

    // -WCRT- tag
    __asm {
        test  eax, 0x0a0d0a0d
        sub   eax, 0x54524357
        sub   eax, 0x0a0d0a0d
    }

    res = DllMain(hinstDLL, fdwReason, lpvReserved);

    if (fdwReason == DLL_PROCESS_DETACH)
    {
        // call functions registered with atexit
        wcrt_perform_atexit();

        // call C pre-termination functions
        _initterm(__xp_a, __xp_z, 0);

        // call C termination functions
        _initterm(__xt_a, __xt_z, 0);
    }

    return res;
}
