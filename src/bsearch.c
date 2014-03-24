/*
 * WCRT  -  Win32API CRT
 *
 * stdlib bsearch
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

void *bsearch(const void *key,
              const void *base,
              size_t n,
              size_t size,
              int (*cmp)(const void *keyval, const void *datum))
{
   const char *l = (const char *) base;
   const char *u = l + (n - 1)*size;

   if (size == 0) return NULL;

   while (l <= u)
   {
       const char *m = l + (((u - l)/size)/2)*size;
       int d = cmp(key, m);

       if (d > 0)
           l = m + size;
       else if (d < 0)
           u = m - size;
       else
           return (char *) m;
   }

   return NULL;
}
