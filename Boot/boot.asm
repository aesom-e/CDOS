[bits 16]
[org 0x7c00]
jmp boot

; Reserve space for the boot info table
times 0x40 db 0x00

%include "Boot/constants.asm"
%include "Boot/print.asm"
%include "Boot/disk.asm"
%include "Boot/gdt.asm"
%include "Boot/protectedMode.asm"
%include "Boot/kernel.asm"

[bits 16]
boot:
    ; Clear the screen
    call print_Clear

    ; Disable the cursor
    pusha
    mov ah, 0x01
    mov ch, 0x20
    int 0x10
    popa

    ; Store the boot drive
    mov [BOOT_DRIVE], dl

    ; Setup the stack
    mov bp, 0x7bff
    mov sp, bp

    ; Alert the user that the boot has started
    mov bx, MESSAGE_BOOT
    call print_Bx
    call print_Newline

    ; Load the kernel
    call disk_LoadKernel

    ; Start protected mode
    call protectedMode_Start

    ; Halt
    jmp $

times 510-($-$$) db 0
dw 0xaa55