section .text

global HT_Insert
extern Add_Node

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
HT_Insert:

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

        mov     rcx, qword [rdi + 16]       ; rcx = ht_ptr->size

        xor     rdx, rdx                    ; hash = hash % ht_ptr->size
        div     rcx                         ; 
        mov     rax, rdx                    ; rax = hash
        sal     rax, 3

        mov     rdx, qword [rdi]            ; rdx = ht_ptr->array
        mov     r11, qword [rdx + rax]      ; r11 = ht_ptr->array[hash]

        test    r11, r11                    ; ht_ptr->array[hash] ?= NULL
        jne     .not_empty                  ; if != contunue

        mov     rdi, rsi                    ; rdi = data

        push    rax
        push    rdx

        call    Add_Node

        pop     rdx
        pop     rcx

        mov     qword [rdx + rcx], rax      ; ht_ptr->array[hash] = Add_Node(data)
        jmp     .return

.not_empty:
                                            ; r11: current
        mov     rcx, qword [r11 + 16]       ; rcx: next = current->next

        xor     r8, r8                      ; r8: node_i = 0
        jmp     .condition

.while:
        mov     r11, rcx                    ; current = next
        mov     rcx, qword [r11 + 16]       ; next = current->next

.condition:
        test    rcx, rcx
        jne     .while

        mov     rdi, rsi
        push    r11
        call    Add_Node
        pop     r11
        mov     qword [r11 + 16], rax

.return:
        ret
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
