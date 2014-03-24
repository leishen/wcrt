;;
;; WCRT  -  Win32API CRT
;;
;; _chkstk
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

public _chkstk, __chkstk

.code

; =============================================================

_chkstk:
__chkstk:
    ; _chkstk, called with eax = space required

    push   edx                ; save edx
    lea    edx, [esp + 8]     ;
    sub    edx, eax           ; edx = target esp

    ; in theory the first test below could access a page
    ; outside the stack space, but that's highly unlikely
    ; to ever happen (famous last words ;).

  touchloop:
    test   [edx + eax], ecx   ; probe
    sub    eax, 4096          ; move down one page
    jae    touchloop          ; done?

    ; we could do a final probe at the target address by using ja
    ; above and doing 'test [edx], ecx', but since no _chkstk call
    ; is generated when allocating less than a page, the generated
    ; code shouldn't rely on the target address being probed.

    pop    eax                ; eax = original edx
    xchg   eax, edx           ; restore edx, eax = target esp
    xchg   eax, esp           ; esp = target esp, eax -> return addr

    jmp    dword ptr [eax]    ; ret

; =============================================================

end
