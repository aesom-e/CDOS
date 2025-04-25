; System messages
MESSAGE_BOOT        db "[Boot Started]", 0
MESSAGE_HALT        db "[System Halted]", 0
MESSAGE_DISKERROR   db "[Unknown Disk Error]", 0
MESSAGE_SECTORERROR db "[Incorrect Number Of Sectors Loaded]", 0
MESSAGE_KERNELLOAD  db "[Kernel Loaded]", 0

; Kernel constants
KERNEL_LOCATION      equ 0x7e00 ; Where the kernel should be placed
KERNEL_SIZE          equ 0x1f   ; Size of the kernel in 2048 byte chunks
KERNEL_DISK_LOCATION equ 0x22   ; The chunk that the kernel begins at in the disk

; Other
BOOT_DRIVE db 0