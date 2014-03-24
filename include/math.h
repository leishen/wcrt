/*
 * WCRT  -  Win32API CRT
 *
 * math.h
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

#ifndef WCRT_MATH_H_INCLUDED
#define WCRT_MATH_H_INCLUDED

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

 /* TODO:
    double frexp(double x, int *exp);
    double modf(double x, double *ip);
    double fmod(double x, double y);
 */

    double sin(double x);
    double cos(double x);
    double tan(double x);
    double asin(double x);
    double acos(double x);
    double atan(double x);
    double atan2(double y, double x);
    double sinh(double x);
    double cosh(double x);
    double tanh(double x);
    double exp(double x);
    double exp2(double x);
    double log(double x);
    double log10(double x);
    double log2(double x);
    double pow(double x, double y);

    double sqrt(double x);
    double ceil(double x);
    double floor(double x);
    double round(double x);
    double trunc(double x);
    double fabs(double x);
    double ldexp(double x, int n);

    float sinf(float x);
    float cosf(float x);
    float tanf(float x);
    float asinf(float x);
    float acosf(float x);
    float atanf(float x);
    float atan2f(float y, float x);
    float sinhf(float x);
    float coshf(float x);
    float tanhf(float x);
    float expf(float x);
    float exp2f(float x);
    float logf(float x);
    float log10f(float x);
    float log2f(float x);
    float powf(float x, float y);

    float sqrtf(float x);
    float ceilf(float x);
    float floorf(float x);
    float roundf(float x);
    float truncf(float x);
    float fabsf(float x);
    float ldexpf(float x, int n);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WCRT_MATH_H_INCLUDED */
