extern isr_Handler

isr_common_stub:
    ; Save the CPU state
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call the handler
    push esp
    call isr_Handler
    pop eax

    ; Restore the state
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 0x08
    iret

; Define the ISRs
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; Division by zero
isr0:
    push byte 0
    push byte 0
    jmp isr_common_stub

; Debug
isr1:
    push byte 0
    push byte 1
    jmp isr_common_stub

; NMI exception
isr2:
    push byte 0
    push byte 2
    jmp isr_common_stub

; Breakpoint
isr3:
    push byte 0
    push byte 3
    jmp isr_common_stub

; Overflow
isr4:
    push byte 0
    push byte 4
    jmp isr_common_stub

; Out of bounds
isr5:
    push byte 0
    push byte 5
    jmp isr_common_stub

; Invalid opcode
isr6:
    push byte 0
    push byte 6
    jmp isr_common_stub

; Coprocessor not available
isr7:
    push byte 0
    push byte 7
    jmp isr_common_stub

; Double fault (contains an error code)
isr8:
    push byte 8
    jmp isr_common_stub

; Coprocessor segment overrun
isr9:
    push byte 0
    push byte 9
    jmp isr_common_stub

; Bad TSS (contains an error code)
isr10:
    push byte 10
    jmp isr_common_stub

; Segment not present (contains an error code)
isr11:
    push byte 11
    jmp isr_common_stub

; Stack fault exception (contains an error code)
isr12:
    push byte 12
    jmp isr_common_stub

; General protection fault (contains an error code)
isr13:
    push byte 13
    jmp isr_common_stub

; Page fault (contains an error code)
isr14:
    push byte 14
    jmp isr_common_stub

; Unknown interrupt
isr15:
    push byte 0
    push byte 15
    jmp isr_common_stub

; Floating point exception
isr16:
    push byte 0
    push byte 16
    jmp isr_common_stub

; Allignment check
isr17:
    push byte 0
    push byte 17
    jmp isr_common_stub

; Machine check
isr18:
    push byte 0
    push byte 18
    jmp isr_common_stub

; Reserved
isr19:
    push byte 0
    push byte 19
    jmp isr_common_stub

; Reserved
isr20:
    push byte 0
    push byte 20
    jmp isr_common_stub

; Reserved
isr21:
    push byte 0
    push byte 21
    jmp isr_common_stub

; Reserved
isr22:
    push byte 0
    push byte 22
    jmp isr_common_stub

; Reserved
isr23:
    push byte 0
    push byte 23
    jmp isr_common_stub

; Reserved
isr24:
    push byte 0
    push byte 24
    jmp isr_common_stub

; Reserved
isr25:
    push byte 0
    push byte 25
    jmp isr_common_stub

; Reserved
isr26:
    push byte 0
    push byte 26
    jmp isr_common_stub

; Reserved
isr27:
    push byte 0
    push byte 27
    jmp isr_common_stub

; Reserved
isr28:
    push byte 0
    push byte 28
    jmp isr_common_stub

; Reserved
isr29:
    push byte 0
    push byte 29
    jmp isr_common_stub

; Reserved
isr30:
    push byte 0
    push byte 30
    jmp isr_common_stub

; Reserved
isr31:
    push byte 0
    push byte 31
    jmp isr_common_stub