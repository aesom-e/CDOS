[bits 16]
protectedMode_Start:
    cli                   ; Disable interrupts
    lgdt [gdt_descriptor] ; Load the GDT

    ; Swtich on the protected mode flag
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    ; Far jump to initialize everything needed
    jmp CODE_SEG:protectedMode_Init

[bits 32]
protectedMode_Init:
    ; Update segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Setup the stack
    mov ebp, 0x90000
    mov esp, ebp

    ; Call the kernel
    call kernel_Start