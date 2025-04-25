[bits 32]
kernel_Start:
    ; The true position of the kernel is 0x200 away from where it's loaded
    mov eax, KERNEL_LOCATION
    add eax, 0x200

    call eax

    jmp $