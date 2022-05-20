section .text

global HT_Search        

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; input:                                        ;
; ~~~~~~                                        ;
;       rdi: ht_ptr                             ;
;       rsi: data                               ;
;                                               ;
; output:                                       ;
; ~~~~~~~                                       ;
;       rax                                     ;
;                                               ;
; destructed:                                   ;
; ~~~~~~~~~~                                    ;
;       rax, rdx, rcx, r8                       ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;

HT_Search:

        or      rax, 0xFFFFFFFFFFFFFFFF         ; crc-32 initial value

        crc32   rax, qword [rsi]
        crc32   rax, qword [rsi + 8]
        crc32   rax, qword [rsi + 16]
        crc32   rax, qword [rsi + 24]        
        
        not     rax                             ; rax: hash

        xor     rdx, rdx                        ;
        div     dword [rdi + 8]                 ; rdx = hash % ht_ptr->size

        mov     rax, qword [rdi]                ; rax = ht_ptr->array
        mov     rdx, qword [rax + rdx * 8]      ; rdx = ht_ptr->array[hash]

        test    rdx, rdx                        ; ht_ptr->array[hash] ?= NULL
        je      .not_found                      ; if not, contunue
                                            
                                                ; rdx: current
        mov     rcx, qword [rdx + 8]            ; rcx: next = current->next
        xor     r8, r8

.for:
        test    rcx, rcx
        je      .next_null
                                                ; rsi: 2nd argument: data
        mov     rdi, qword [rdx]                ; 1st argument: currect->str

        vmovdqu ymm1, yword [rdi]               ; ymm1: str_1

        vpcmpeqb ymm0, ymm1, yword [rsi]        ; compares str_1 and str_2
                                                ; puts mask in ymm0
        vpmovmskb eax, ymm0

        cmp     eax, 0xFFFFFFFF
        jne     .not_equal

        mov     rax, r8
        jmp     .return

.not_equal:
        mov     rdx, rcx                        ; current = next
        mov     rcx, qword [rdx + 8]            ; next = current->next
        inc     r8

        jmp     .for

.next_null:
                                                ; rsi: 2nd argument: data
        mov     rdi, qword [rdx]                ; 1st argument: currect->str

        vmovdqu ymm1, yword [rdi]

        vpcmpeqb ymm0, ymm1, yword [rsi]
        
        vpmovmskb eax, ymm0

        cmp     eax, 0xFFFFFFFF
        jne     .not_found

        mov     rax, r8
        jmp     .return

.not_found:
        mov     rax, -1

.return:
        ret
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
