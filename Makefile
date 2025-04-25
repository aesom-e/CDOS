# Compiler and assembler settings
ASM_COMPILER = nasm
ASM_FLAGS = -f bin
ASM_OBJ_FLAGS = -f elf

CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs

LD = ld
LDFLAGS = -m elf_i386 -Ttext 0x8000 --oformat binary

# Files and directories
SRC_DIR = Boot
BUILD_DIR = Build
BOOT_BUILD_DIR = $(BUILD_DIR)/Boot
KERNEL_BUILD_DIR = $(BUILD_DIR)/Kernel
ISO_ROOT = $(BUILD_DIR)/iso
ISO_FILE = $(BUILD_DIR)/os.iso
BOOT_FILE = $(SRC_DIR)/boot.asm
BOOT_BIN = $(BOOT_BUILD_DIR)/boot.bin
OS_BIN = $(BUILD_DIR)/os.bin

# Kernel-related
KERNEL_DIR = Kernel
ENTRY_FILE = $(KERNEL_DIR)/entry.asm
ENTRY_OBJ = $(KERNEL_BUILD_DIR)/entry.o
KERNEL_C_FILES = $(shell find $(KERNEL_DIR) -type f -name "*.c")
KERNEL_ASM_FILES = $(filter-out $(ENTRY_FILE), $(shell find $(KERNEL_DIR) -type f -name "*.asm"))
KERNEL_OBJ_FILES = $(patsubst $(KERNEL_DIR)/%.c, $(KERNEL_BUILD_DIR)/%.o, $(KERNEL_C_FILES))
KERNEL_ASM_OBJ_FILES = $(patsubst $(KERNEL_DIR)/%.asm, $(KERNEL_BUILD_DIR)/%Asm.o, $(KERNEL_ASM_FILES))
KERNEL_BIN = $(KERNEL_BUILD_DIR)/kernel.bin

default:
	@$(MAKE) clean
	@$(MAKE) all

all: $(ISO_FILE)
	@qemu-system-i386 -cdrom $(ISO_FILE) -m 64M -hda disk.img -boot d -full-screen
	@cp $(ISO_FILE) /mnt/d/CDOS/

$(ISO_FILE): $(OS_BIN)
	@mkdir -p $(ISO_ROOT)
	@cp $(OS_BIN) $(ISO_ROOT)/os.bin
	@xorriso -as mkisofs -R -V "CDOS" -b os.bin -no-emul-boot -boot-load-size 1 -boot-info-table -o $@ $(ISO_ROOT)

$(OS_BIN): $(BOOT_BIN) $(KERNEL_BIN)
	@mkdir -p $(BUILD_DIR)
	@cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_BIN)

$(BOOT_BIN): $(BOOT_FILE)
	@mkdir -p $(BOOT_BUILD_DIR)
	@$(ASM_COMPILER) $(ASM_FLAGS) -o $@ $<

$(KERNEL_BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(KERNEL_BUILD_DIR)/%Asm.o: $(KERNEL_DIR)/%.asm
	@mkdir -p $(dir $@)
	@$(ASM_COMPILER) $(ASM_OBJ_FLAGS) -o $@ $<

$(ENTRY_OBJ): $(ENTRY_FILE)
	@mkdir -p $(dir $@)
	@$(ASM_COMPILER) $(ASM_OBJ_FLAGS) -o $@ $<

$(KERNEL_BIN): $(ENTRY_OBJ) $(KERNEL_OBJ_FILES) $(KERNEL_ASM_OBJ_FILES)
	@mkdir -p $(dir $@)
	@$(LD) $(LDFLAGS) -o $@ $^

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all clean