/*
 * WCRT  -  Win32API CRT
 *
 * heap
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

#ifndef WCRT__HEAP_H_INCLUDED
#define WCRT__HEAP_H_INCLUDED

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WCRT_USE_GLOBALALLOC

# define WCRT_ALLOC(size)      GlobalAlloc(GMEM_FIXED, (size))
# define WCRT_ALLOCZ(size)     GlobalAlloc(GPTR, (size))
# define WCRT_REALLOC(p, size) GlobalReAlloc((HGLOBAL) (p), (size), 0)
# define WCRT_FREE(p)          GlobalFree((HGLOBAL) (p))

#else /* WCRT_USE_GLOBALALLOC */

  extern HANDLE wcrt_heap;

# define WCRT_ALLOC(size)      HeapAlloc(wcrt_heap, 0, (size))
# define WCRT_ALLOCZ(size)     HeapAlloc(wcrt_heap, HEAP_ZERO_MEMORY, (size))
# define WCRT_REALLOC(p, size) HeapReAlloc(wcrt_heap, 0, (p), (size))
# define WCRT_FREE(p)          HeapFree(wcrt_heap, 0, (p))

#endif /* WCRT_USE_GLOBALALLOC */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WCRT__HEAP_H_INCLUDED */
