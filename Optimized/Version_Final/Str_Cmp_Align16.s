section .text
global Str_Cmp_Align16

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; input:                                    ;
; ~~~~~~                                    ; 
;       rdi: str_1                          ;
;       rsi: str_2                          ;
;       rdx: len                            ;
; output:                                   ;
; ~~~~~~~                                   ;
;       rax                                 ;
; destructed:                               ;
; ~~~~~~~~~~                                ;
;       rax, r9, r10, rdx                   ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
Str_Cmp_Align16:
    
        xor     r10, r10                    ; chars counter
        mov     r9, rdx                     ; saving len

        mov     rax, 16                     ; len_1 for pcmpestri          
        mov     rdx, 16                     ; len_2 for pcmpestri

        jmp     .condition

.for:

        movaps  xmm1, [rdi + r10]           ; loading str_1
        movaps  xmm2, [rsi + r10]           ; loading str_2

        pcmpestri xmm1, xmm2, 00001100b ; ---+
        ;                                    |
        ;                                    | LOOK HERE (find more in intel docs)
        ;                                   \|/
        ;                                    *
        ;               no effect -> 0 0 0 0 1 1 0 0 b
        ;                              \./ | \./ |  \.
        ;                          ____/   |  |   \.  128-bit sources are treated as 16 packed bytes
        ; index encodes least    /         |   \.   bytes are unsigned
        ; significant byte of res2        /      mode is equal ordered
        ;                           res2 = res1  

        test    ecx, ecx                    ; checking if string are equal
        jne     .not_equal

        add     r10, 16                     ; move to the next 16 bytes

.condition:
        cmp     r10, r9                     ; compare up to the end of str_1
        jb      .for

        xor     rax, rax                    ; return 0, if equal
        ret

.not_equal:
        mov     rax, 1                      ; return 1, if not equal
        ret   

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
