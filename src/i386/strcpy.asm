;;
;; WCRT  -  Win32API CRT
;;
;; string strcpy
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

public strcpy

.code

; =============================================================

strcpy:
    ; char *strcpy(char *s, const char *ct)

    _ct$ = 8
    _s$  = 4

    mov    ecx, [esp + _s$]
    mov    edx, [esp + _ct$]

    push   ecx

  @@:
    mov    al, [edx]
    inc    edx
    mov    [ecx], al
    inc    ecx
    test   al, al
    jnz    @B

    pop    eax

    ret

; =============================================================

end
