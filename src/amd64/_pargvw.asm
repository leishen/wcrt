;;
;; WCRT  -  Win32API CRT
;;
;; wcrt_parse_argvw
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

public wcrt_parse_argvw

.code

; =============================================================

wcrt_parse_argvw:
    ; wcrt_parse_argv(const wchar_t *cl, int *argc,
    ;                 wchar_t **argv, wchar_t *buffer)

    ; NOTE: This function assumes that argv != NULL means that
    ;       buffer != NULL.

    ; NOTE: Defining SIMPLE will remove the code for backslash
    ;       handling. Quoted strings are still handled, and """
    ;       still works to insert a quote. It cuts the function
    ;       from 153 bytes to 100 bytes.

    _cl$   = 4
    _argc$ = 8
    _argv$ = 12
    _buf$  = 16

    push   esi
    push   edi
    push   ebx

    xor    eax, eax           ; eax = 0
    cdq                       ; edx will count the required space

    mov    esi, _cl$[esp+12]
    mov    ecx, _argv$[esp+12]
    mov    edi, _buf$[esp+12]

    push   edx                ; push zero on stack for argc

  skip_ws:
    lodsw

  next_arg:
    test   eax, eax
    jz     done

    cmp    eax, ' '
    jbe    skip_ws

    dec    esi                ; adjust esi so we re-read first non-ws
    dec    esi                ;

    xor    eax, eax

    jecxz  @F
    stosw                     ; zero-terminate previous arg
    mov    [ecx], edi         ; store pointer in argv
    add    ecx, 4             ;
  @@:

    inc    dword ptr [esp]    ; increment argc

    add    edx, 6             ; need 6 more bytes

    xor    ebx, ebx           ; initialize arg terminator to space
    mov    bl, ' '            ;

  copy_arg:
    lodsw

    ; quote handling

  quote_check:
    cmp    eax, '"'
    jne    bs_check

    xor    ebx, ' '           ; toggle terminator between space and 0
    jz     copy_arg           ; if opening quote, continue as normal

    lodsw

    ; backslash handling

  bs_check:

ifndef SIMPLE

    cmp    eax, '\'           ; check for '\'
    jne    test_and_write     ;

    push   ecx

    xor    ecx, ecx           ; clear counter

  bs_more:
    inc    ecx                ; count number of '\'
    lodsw                     ;
    cmp    eax, '\'           ;
    je     bs_more            ;

    dec    esi                ; adjust esi so we re-read first non-'\'
    dec    esi                ;

    cmp    eax, '"'           ; if next char is not a '"', write all '\'
    jne    bs_write           ;

    shr    ecx, 1             ; half the number of '\'
    jnc    bs_write           ; if uneven we have an escaped '"'

    dec    esi                ; go back to last '\'
    dec    esi                ;

  bs_write:
    add    edx, ecx           ; need ecx more bytes

    xor    eax, eax

    cmp    [esp], eax
    mov    al, '\'
    je     @F
    rep    stosw              ; store ecx times '\'
  @@:

    pop    ecx

    lodsw

    cmp    eax, '\'           ; if not at '\' we need to handle
    jne    quote_check        ; this char normaly

    lodsw                     ; read '"'

endif

    ; check for end of argument and write

  test_and_write:
    cmp    eax, ebx           ; have we reached end of current arg?
    jbe    next_arg           ;

  write_char:
    jecxz  @F
    stosw                     ; store char in buffer
  @@:

    inc    edx                ; need 2 more byte
    inc    edx                ;
    jmp    copy_arg

  done:
    jecxz  @F
    stosd                     ; zero-terminate last arg
    mov    [ecx], eax         ; zero-terminate argv array
  @@:

    lea    eax, [edx + 16]    ; eax = required space

    mov    ecx, _argc$[esp+16]

    pop    edx                ; edx = argc

    jecxz  @F
    mov    [ecx], edx         ; store argc
  @@:

    pop    ebx
    pop    edi
    pop    esi

    ret

; =============================================================

end
