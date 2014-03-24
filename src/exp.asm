;;
;; WCRT  -  Win32API CRT
;;
;; math exp and epxf
;;
;; Copyright (c) 2003-2004 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;
;; http://www.ibsensoftware.com/
;;
;; This software is provided 'as-is', without any express
;; or implied warranty.  In no event will the authors be
;; held liable for any damages arising from the use of
;; this software.
;;
;; Permission is granted to anyone to use this software
;; for any purpose, including commercial applications,
;; and to alter it and redistribute it freely, subject to
;; the following restrictions:
;;
;; 1. The origin of this software must not be
;;    misrepresented; you must not claim that you
;;    wrote the original software. If you use this
;;    software in a product, an acknowledgment in
;;    the product documentation would be appreciated
;;    but is not required.
;;
;; 2. Altered source versions must be plainly marked
;;    as such, and must not be misrepresented as
;;    being the original software.
;;
;; 3. This notice may not be removed or altered from
;;    any source distribution.
;;

.386
.model flat,c

option casemap:none

public exp
public expf

.code

; =============================================================

exp:
    ; double exp(double x)

    _x$ = 4

    fld   real8 ptr [esp + _x$]

    jmp   @F

expf:
    ; float expf(float x)

    fld   real4 ptr [esp + _x$]

  @@:
    fldl2e

    fmul                      ; ST = x*log2(e)

    fld   st                  ; ST(1) = ST
    frndint                   ; ST    = integer part
    fsub  st(1), st           ; ST(1) = fractional part
    fxch  st(1)               ; swap

    f2xm1                     ; compute result for fraction

    fld1                      ; correct for -1 of f2xm1
    fadd                      ;

    fscale                    ; correct for integer part

    fstp  st(1)               ; adjust stack

    ret

; =============================================================

end
