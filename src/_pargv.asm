;;
;; WCRT  -  Win32API CRT
;;
;; wcrt_parse_argv
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

public wcrt_parse_argv

.code

; =============================================================

wcrt_parse_argv:
    ; wcrt_parse_argv(const char *cl, int *argc,
    ;                 char **argv, char *buffer)

    ; NOTE: This function assumes that argv != NULL means that
    ;       buffer != NULL.

    ; NOTE: Defining SIMPLE will remove the code for backslash
    ;       handling. Quoted strings are still handled, and """
    ;       still works to insert a quote. It cuts the function
    ;       from 130 bytes to 88 bytes.

    _cl$   = 4
    _argc$ = 8
    _argv$ = 12
    _buf$  = 16

    push   esi
    push   edi

    xor    edx, edx           ; edx will count the required space

    mov    esi, _cl$[esp+8]
    mov    ecx, _argv$[esp+8]
    mov    edi, _buf$[esp+8]

    push   edx                ; push zero on stack for argc

  skip_ws:
    lodsb

  next_arg:
    test   al, al
    jz     done

    cmp    al, ' '
    jbe    skip_ws

    dec    esi                ; adjust esi so we re-read first non-ws

    xor    eax, eax

    jecxz  @F
    stosb                     ; zero-terminate previous arg
    mov    [ecx], edi         ; store pointer in argv
    add    ecx, 4             ;
  @@:

    inc    dword ptr [esp]    ; increment argc

    add    edx, 5             ; need 5 more bytes

    mov    ah, ' '            ; initialize arg terminator to space

  copy_arg:
    lodsb

    ; quote handling

  quote_check:
    cmp    al, '"'
    jne    bs_check

    xor    ah, ' '            ; toggle terminator between space and 0
    jz     copy_arg           ; if opening quote, continue as normal

    lodsb

    ; backslash handling

  bs_check:

ifndef SIMPLE

    cmp    al, '\'            ; check for '\'
    jne    test_and_write     ;

  bs_parse:
    push   ecx

    xor    ecx, ecx           ; clear counter

  bs_more:
    inc    ecx                ; count number of '\'
    lodsb                     ;
    cmp    al, '\'            ;
    je     bs_more            ;

    dec    esi                ; adjust esi so we re-read first non-'\'

    cmp    al, '"'            ; if next char is not a '"', write all '\'
    jne    bs_write           ;

    shr    ecx, 1             ; half the number of '\'
    jnc    bs_write           ; if uneven we have an escaped '"'

    dec    esi                ; go back to last '\'

  bs_write:
    add    edx, ecx           ; need ecx more bytes

    mov    al, '\'

    cmp    dword ptr [esp], 0
    jz     @F
    rep    stosb              ; store ecx times '\'
  @@:

    pop    ecx

    lodsb

    cmp    al, '\'            ; if not at '\' we need to handle
    jne    quote_check        ; this char normaly

    lodsb                     ; read '"'

endif

    ; check for end of argument and write

  test_and_write:
    cmp    al, ah             ; have we reached end of current arg?
    jbe    next_arg           ;

  write_char:
    jecxz  @F
    stosb                     ; store char in buffer
  @@:

    inc    edx                ; need 1 more byte
    jmp    copy_arg

  done:
    jecxz  @F
    xor    eax, eax
    stosd                     ; zero-terminate last arg
    mov    [ecx], eax         ; zero-terminate argv array
  @@:

    lea    eax, [edx + 16]    ; eax = required space

    mov    ecx, _argc$[esp+12]

    pop    edx                ; edx = argc

    jecxz  @F
    mov    [ecx], edx         ; store argc
  @@:

    pop    edi
    pop    esi

    ret

; =============================================================

end
