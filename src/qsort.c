/*
 * WCRT  -  Win32API CRT
 *
 * stdlib qsort
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

#include <stdio.h>

typedef int (*compare_func)(const void *, const void *);

#define THRESHOLD (0)

// Better worst-case, but bigger.
// #define WCRT_USE_INTROSORT

// Slower, but smaller.
// #define WCRT_USE_INSERTIONSORT

/*

// Depending on which operation is most costly, compare or
// swap, it can be better to use a selection sort at the
// bottom on the quicksort recursion.

static void selectionsort(char *l,
                          char *u,
                          size_t size,
                          compare_func cmp)
{
    while (l < u)
    {
        char *p = l;
        char *c;

        for (c = u; c > l; c -= size)
        {
            if (cmp(c, p) < 0) p = c;
        }

        swap(l, p, size);

        l += size;
    }
}

*/

static void swap(char *op1,
                 char *op2,
                 size_t size)
{
    if (op1 == op2) return;

    while (size > 3)
    {
        unsigned int tmp = *(unsigned int *)op1;
        *((unsigned int *)op1)++ = *(unsigned int *)op2;
        *((unsigned int *)op2)++ = tmp;

        size -= 4;
    }

    while (size--)
    {
        char tmp = *op1;
        *op1++ = *op2;
        *op2++ = tmp;
    }
}

#ifdef WCRT_USE_INSERTIONSORT

// There are no requirements about the running time of the qsort
// function in the C standard, so if size is very important, we
// can just use insertion sort.

void qsort(void *base,
           size_t n,
           size_t size,
           compare_func cmp)
{
    char *l = (char *)base;
    char *u = l + (n - 1)*size;

    if ((n < 2) || (size == 0)) return;

    for (l += size; l <= u; l += size)
    {
        char *c;

        for (c = l; c > (char *)base; c -= size)
        {
            if (cmp(c - size, c) <= 0) break;
            swap(c - size, c, size);
        }
    }
}

#else /* WCRT_USE_INSERTIONSORT */

static char *partition(char *l,
                       char *u,
                       size_t size,
                       compare_func cmp)
{
    char *pivot;

#if (THRESHOLD == 0)
    // handle two elements
    if (l + size == u)
    {
        if (cmp(l, u) > 0) swap(l, u, size);
        return u;
    }
#endif

    // pivot is middle element
    pivot = l + (((u - l)/size)/2)*size;

    // order first, middle and last elements,
    // so pivot points to the median of these three
    if (cmp(l, u) > 0) swap(l, u, size);
    if (cmp(pivot, u) > 0) swap(pivot, u, size);
    if (cmp(l, pivot) > 0) swap(l, pivot, size);

    // - pivot does not point to the first or last element
    // - first is <= pivot, last is >= pivot,

    // partition by pivot
    while (1)
    {
        do l += size; while (cmp(l, pivot) < 0);
        do u -= size; while (cmp(pivot, u) < 0);

        if (l >= u) return l;

        swap(l, u, size);

        // adjust pivot, in case we moved the element
        if (pivot == u) pivot = l;
        else if (pivot == l) pivot = u;
    }
}

static void insertionsort(char *l,
                          char *u,
                          size_t size,
                          compare_func cmp)
{
    char *base = l;

    for (l += size; l <= u; l += size)
    {
        char *c;

        for (c = l; c > base; c -= size)
        {
            if (cmp(c - size, c) <= 0) break;

            swap(c - size, c, size);
        }
    }
}

#ifdef WCRT_USE_INTROSORT

// Introsort by David R. Musser. A quicksort that falls back to a
// heapsort if the recursion goes too deep. This results in average
// case time close to that of regular quicksort, and O(n*log(n))
// worst case.

static void movedown(char *base,
                     char *l,
                     char *u,
                     size_t size,
                     compare_func cmp)
{
    // get child
    char *c = base + 2*(l - base) + size;

    while (c < u)
    {
        // choose largest child
        if (cmp(c + size, c) > 0) c += size;

        // stop if we are at the right position
        if (cmp(l, c) > 0) break;

        // swap with child
        swap(l, c, size);

        // continue at child
        l = c;
        c = base + 2*(c - base) + size;
    }

    // do last check and swap if required
    if (c == u)
    {
        if (cmp(l, c) < 0) swap(l, c, size);
    }
}

static void heapsort(char *l,
                     char *u,
                     size_t size,
                     compare_func cmp)
{
    size_t n = ((u - l) / size) + 1;
    char *p;

    // build heap
    for (p = l + (n/2 - 1)*size; p >= l; p -= size)
        movedown(l, p, u, size, cmp);

    // take out elements in sorted order
    for (p = u; p > l; p -= size)
    {
        swap(l, p, size);
        movedown(l, l, p - size, size, cmp);
    }
}

static void introsort_loop(char *l,
                           char *u,
                           size_t size,
                           compare_func cmp,
                           size_t depth)
{
    while ((size_t)(u - l) > THRESHOLD*size)
    {
        char *cut;

        // if recursion has gone too deep, fall back to heapsort
        if (depth == 0)
        {
            heapsort(l, u, size, cmp);
            return;
        }

        depth /= 2;

        cut = partition(l, u, size, cmp);

        introsort_loop(cut, u, size, cmp, depth);
        u = cut - size;
    }
}

void qsort(void *base,
           size_t n,
           size_t size,
           compare_func cmp)
{
    char *l = (char *)base;
    char *u = l + (n - 1)*size;

    if ((n < 2) || (size == 0)) return;

    introsort_loop(l, u, size, cmp, 2*n);

#if (THRESHOLD > 0)
    insertionsort(l, u, size, cmp);
#endif
}

#else /* WCRT_USE_INTROSORT */

/*
static void quicksort_loop(char *l,
                           char *u,
                           size_t size,
                           compare_func cmp)
{
    while ((size_t)(u - l) > THRESHOLD*size)
    {
        char *cut = partition(l, u, size, cmp);

        // recurse on shortest sub-array to keep stack-usage logarithmic
        if (cut - l > u - cut)
        {
            quicksort_loop(cut, u, size, cmp);
            u = cut - size;
        } else {
            quicksort_loop(l, cut - size, size, cmp);
            l = cut;
        }
    }
}
*/

static void quicksort_loop(char *l,
                           char *u,
                           size_t size,
                           compare_func cmp)
{
    while ((size_t)(u - l) > THRESHOLD*size)
    {
        char *pl = l;
        char *pu = u;
        char *pivot;

#if (THRESHOLD == 0)
        // handle two elements
        if (pl + size == pu)
        {
            if (cmp(pl, pu) > 0) swap(pl, pu, size);
            return;
        }
#endif

        // pivot is middle element
        pivot = pl + (((pu - pl)/size)/2)*size;

        // order first, middle and last elements,
        // so pivot points to the median of these three
        if (cmp(pl, pu) > 0) swap(pl, pu, size);
        if (cmp(pivot, pu) > 0) swap(pivot, pu, size);
        if (cmp(pl, pivot) > 0) swap(pl, pivot, size);

        // - pivot does not point to the first or last element
        // - first is <= pivot, last is >= pivot,

        // partition by pivot
        while (1)
        {
            do pl += size; while (cmp(pl, pivot) < 0);
            do pu -= size; while (cmp(pivot, pu) < 0);

            if (pl >= pu) break;

            swap(pl, pu, size);

            // adjust pivot, in case we moved the element
            if (pivot == pu) pivot = pl;
            else if (pivot == pl) pivot = pu;
        }

        // move pu past equal elements
        while ((pu > l) && (cmp(pivot, pu) == 0)) pu -= size;

        // recurse on shortest sub-array to keep stack-usage logarithmic
        if (pu - l > u - pl)
        {
            quicksort_loop(pl, u, size, cmp);

            u = pl - size;

        } else {

            quicksort_loop(l, pu, size, cmp);

            l = pl;
        }

    }
}

void qsort(void *base,
           size_t n,
           size_t size,
           compare_func cmp)
{
    char *l = (char *)base;
    char *u = l + (n - 1)*size;

    if ((n < 2) || (size == 0)) return;

    quicksort_loop(l, u, size, cmp);

#if (THRESHOLD > 0)
    insertionsort(l, u, size, cmp);
#endif
}

#endif /* WCRT_USE_INTROSORT */

#endif /* WCRT_USE_INSERTIONSORT */
