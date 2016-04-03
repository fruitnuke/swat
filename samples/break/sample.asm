section .text
        global _start

_start:
        mov ecx, msg1
        mov edx, len
        mov eax, 4
        mov ebx, 1
        int 0x80

        mov ecx, msg2
        mov edx, len
        mov eax, 4
        mov ebx, 1
        int 0x80

        mov eax, 1
        int 0x80

section .data
        msg1 db 'Hello ', 0xa
        msg2 db 'World.', 0xa
        len equ $ - msg2
