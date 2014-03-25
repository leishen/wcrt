;;
;; WCRT  -  Win32API CRT
;;
;; wcrt_fpinit
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

public _fltused
public _FPinit
public wcrt_fpinit

.data?

_fltused dd ?

.data

_FPinit dd wcrt_fpinit

.code

; =============================================================

wcrt_fpinit:
    ; int wcrt_fpinit()

    ; Coprocessor Check

    ; From Intel Application Note AP-485:
    ; Intel Processor Identification and the CPUID Instruction

    ; The algorithm is to determine whether the floating-point
    ; status and control words are present. If not, no
    ; coprocessor exists.

    push   027F5A5Ah          ; esp -> word 5A5A, esp+2 -> word 027F

    xor    eax, eax           ; eax = FALSE

    fninit                    ; reset FP status word
    fnstsw [esp]              ; save FP status word
    cmp    [esp], al          ; check FP status word
    jne    end_fpu_check      ; no FPU present

    fnstcw [esp]              ; save FP control word
    mov    dx, 103Fh          ; selected parts to examine
    and    edx, [esp]         ; check FP control word
    cmp    dx, 3Fh            ; was control word correct
    jne    end_fpu_check      ; incorrect control word, no FPU

    inc    eax                ; eax = TRUE

    fldcw  [esp + 2]          ; set precision to 53 bits (C default)

  end_fpu_check:
    pop    edx                ; remove temporary from stack

    ret

; =============================================================

end
