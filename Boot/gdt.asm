[bits 16]

gdt_start:
gdt_null:
        dq 0x0
gdt_code:
        dw 0xffff     ; First 16 bits of the Segment limit
        dw 0x0000     ; First 16 bits of the base address
        db 0x00       ; Last 8 bits of the base address
        ; Explanation:
        ; 1  - Present bit (1 means it's present)
        ; 00 - Descriptor privilege level (00 means ring 0)
        ; 1  - Descriptor type (1 means code/data, 0 means system)
        ; 1  - Executable bit (1 means it's executable)
        ; 0  - Direction bit (0 means that the code grows up)
        ; 1  - Readable bit (1 means it's readable)
        ; 0  - Accessed bit (0 means it hasn't been accessed)
        db 0b10011010
        ; Explanation:
        ; 1    - Granularity bit (1 means 4kB granularity, 0 means byte granularity)
        ; 1    - Size bit (1 means 32-bit, 0 means 16-bit)
        ; 0    - Long bit (1 means 64-bit, 0 means 32-bit)
        ; 0    - Availability bit (0 means it's not available to the system)
        ; 1111 - Last 4 bits of the segment limit
        db 0b11001111
        db 0x0 ; Segment base bits 24-31
gdt_data:
        dw 0xffff
        dw 0x0000
        db 0x00
        db 0b10010010
        db 0b11001111
        db 0x00


; Here for the size calculation
gdt_end:

gdt_descriptor:
        dw gdt_end - gdt_start - 1
        dd gdt_start


CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start