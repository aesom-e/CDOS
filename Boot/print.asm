[bits 16]
print_Bx:
    pusha
    mov ah, 0x0e

.loop:
    ; Check for NUL-termination
    mov al, [bx]
    cmp al, 0x00
    je .end
    int 0x10
    inc bx
    jmp .loop

.end:
    popa
    ret

[bits 16]
print_Newline:
    push ax
    mov ah, 0x0e
    mov al, 0x0d ; Carriage return
    int 0x10
    mov al, 0x0a ; Newline
    int 0x10
    pop ax
    ret

[bits 16]
print_Clear:
    pusha

    mov ah, 0x00 ; Set video mode
    mov al, 0x03 ; To text mode (80x25, 16 colours, and 8 pages)
    int 0x10

    popa
    ret