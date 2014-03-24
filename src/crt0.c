/*
 * WCRT  -  Win32API CRT
 *
 * crt0
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

extern int __cdecl main(int, char **, char **);

extern unsigned int __cdecl wcrt_parse_argv(const char *cl, int *argc, char **argv, char *buffer);

void __cdecl mainCRTStartup()
{
    unsigned char *cl;
    char *clbuffer;
    int argc;
    int res;

    // check and init fpu if required,
    // exit if required, but not available
    if (_FPinit && (_FPinit() == 0)) ExitProcess(-2);

    // call c initialisers
    if (_initterm(__xi_a, __xi_z, 1)) ExitProcess(-3);

    // call c++ initialisers
    _initterm(__xc_a, __xc_z, 0);

    // get command line
    cl = GetCommandLine();
    if (cl == NULL) cl = "";

    // parse command line into argv array
    {
        int clsize;

        clsize = wcrt_parse_argv(cl, &argc, NULL, NULL);

        clbuffer = (char *) malloc(clsize);

        if (clbuffer)
        {
            wcrt_parse_argv(
                cl,
                &argc,
                (char **)clbuffer,
                clbuffer + (argc + 1)*sizeof(char *)
            );
        } else {
            argc = 0;
        }
    }

    // -WCRT- tag
    __asm {
        test  eax, 0x0a0d0a0d
        sub   eax, 0x54524357
        sub   eax, 0x0a0d0a0d
    }

    // call main
    if (clbuffer)
    {
        res = main(argc, (char **)clbuffer, 0);
        free(clbuffer);

    } else {

        res = main(0, 0, 0);
    }

    exit(res);
}
