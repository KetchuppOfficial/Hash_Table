section .text

global Ded_Hash

;---------------------------------

Ded_Hash:

    movzx rax, byte [rdi]
    xor rcx, rcx

    jmp .condition

.for:

    ror rax, 1
    movzx rbx, byte [rdi + rcx]
    xor rax, rbx
    inc rcx

.condition:

    cmp [rdi + rcx], byte 0
    jne .for

    ret

;---------------------------------
