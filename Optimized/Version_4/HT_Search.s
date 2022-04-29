section .text

global HT_Search
extern Str_Cmp_SSE

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; input:                                    ;
; ~~~~~~                                    ;
;       rdi: ht_ptr                         ;
;       rsi: data                           ;
;                                           ;
; output:                                   ;
; ~~~~~~~                                   ;
;       rax                                 ;
;                                           ;
; destructed:                               ;
; ~~~~~~~~~~                                ;
;       rax, rdx, rcx, r8, r9, r11          ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
HT_Search:

; ~~~~~~~~~~ Ded_Hash ~~~~~~~~~~

        movzx   rax, byte [rsi]
        xor     rcx, rcx

        jmp     .ded_condition

.ded_for:
        ror     rax, 1
        movzx   rdx, byte [rsi + rcx]
        xor     rax, rdx
        inc     rcx

.ded_condition:
        cmp     [rsi + rcx], byte 0
        jne     .ded_for

; ~~~~~~~~~~ Ded_Hash ~~~~~~~~~~

        mov     rcx, qword [rdi + 8]        ; rcx = ht_ptr->size

        xor     rdx, rdx                    ; hash = hash % ht_ptr->size
        div     rcx                         ; 
        mov     rax, rdx                    ; rax = hash
        sal     rax, 3

        mov     rdx, qword [rdi]            ; rdx = ht_ptr->array
        mov     r11, qword [rdx + rax]      ; r11 = ht_ptr->array[hash]

        test    r11, r11                    ; ht_ptr->array[hash] ?= NULL
        jne     .not_empty                  ; if != contunue

        jmp     .not_found

.not_empty:
                                            
        push    rbx
                                            ; r11: current
        mov     rbx, qword [r11 + 16]       ; rcx: next = current->next

        xor     r8, r8                      ; r8: node_i = 0
        jmp     .condition

.for:
        mov     rdx, qword [r11 + 8]        ; 3rd argument: current->len

                                            ; rsi: 2nd argument: data
        mov     rdi, qword [r11]            ; 1st argument: currect->str

        mov     rdi, qword [r11]            ; 1st argument: currect->str
        call    Str_Cmp_SSE

        test    rax, rax
        jne     .not_equal

        mov     rax, r10

        pop     rbx
        jmp     .return

.not_equal:
        mov     r11, rbx
        mov     rbx, qword [r11 + 16]
        inc     r8

.condition:
        test    rbx, rbx
        jne     .for

        pop     rbx

        mov     rdx, qword [r11 + 8]        ; 3rd argument: current->len

                                            ; rsi: 2nd argument: data
        mov     rdi, qword [r11]            ; 1st argument: currect->str

        mov     rdi, qword [r11]            ; 1st argument: currect->str
        call    Str_Cmp_SSE

        test    rax, rax
        jne     .not_found

        mov     rax, rdx
        jmp     .return

.not_found:
        mov     rax, -1

.return:
        ret
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
