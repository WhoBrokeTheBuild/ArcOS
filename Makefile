
CC   ?= clang
LD   ?= clang
ASM  ?= nasm
QEMU ?= qemu-system-i386

CFLAGS += -g -I. -ffreestanding -Wall -Wextra -fno-exceptions -nobuiltininc -m32 -std=c11

OUT    = arcos.bin
BOOT   = boot/boot.bin
KERNEL = kernel/kernel.bin

KERNEL_SRC = $(shell find kernel/ -name '*.c')
KERNEL_OBJ = $(patsubst %.c,%.o,$(KERNEL_SRC))

all: $(OUT)

run: all
	$(QEMU) -drive file=$(OUT),index=0,media=disk,format=raw

clean:
	rm -rf *.bin *.dis *.o *.elf $(OUT)
	rm -rf boot/*.o kernel/*.o

$(OUT): $(BOOT) $(KERNEL)
	cat $^ > $(OUT)

$(KERNEL): boot/kernel_entry.o $(KERNEL_OBJ)
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $< -f elf -o $@

%.bin: %.asm
	$(ASM) $< -f bin -o $@
