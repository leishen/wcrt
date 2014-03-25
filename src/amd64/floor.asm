;;
;; WCRT  -  Win32API CRT
;;
;; math floor and floorf
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

public floor
public floorf

.code

; =============================================================

floor:
    ; double floor(double x)

    _x$ = 4

    fld   real8 ptr [esp + _x$]

    jmp   @F

floorf:
    ; float floorf(float x)

    fld   real4 ptr [esp + _x$]

  @@:
    push  eax                 ; sub esp, 4

    fstcw [esp]               ; save org_cw
    fwait

    pop   eax                 ; create cw with rounding mode
    push  eax                 ; set to round down
    and   ah, 0f3h            ;
    or    ah, 004h            ;

    push  eax                 ; put cw on stack

    fldcw [esp]               ; load cw

    pop   eax                 ; remove cw from stack

    frndint                   ; perform rounding

    fldcw [esp]               ; load org_cw

    pop   eax                 ; add esp, 4

    ret

; =============================================================

end
