[bits 16]
disk_LoadKernel:
    pusha

    ; Prepare to read from the CD-ROM
    mov ah, 0x42         ; Extended read
    mov bx, 0x0000       ; No buffer chaining
    mov dl, [BOOT_DRIVE] ; Drive number

    ; Set up the DAP
    lea si, dap
    int 0x13
    jc .error_General

    ; Tell the user that the kernel was successfully loaded
    mov bx, MESSAGE_KERNELLOAD
    call print_Bx
    call print_Newline

    popa
    ret

.error_General:
    ; Alert the user to the error
    mov bx, MESSAGE_DISKERROR
    call print_Bx
    call print_Newline

    ; Tell the user that the system will halt
    mov bx, MESSAGE_HALT
    call print_Bx
    call print_Newline

    ; Halt
    jmp $

; Disk Access Packet struct
dap:
    db 0x10                 ; Size of the DAP
    db 0x00                 ; Reserved
    dw KERNEL_SIZE          ; Number of sectors to read
    dw KERNEL_LOCATION      ; Location to place the read bytes
    dw 0x0000               ; Segment to place the memory
    dq KERNEL_DISK_LOCATION ; Starting LBA of the kernel