section .text

global _start

_start:
    crc32 rax, qword [rsi + 8]
