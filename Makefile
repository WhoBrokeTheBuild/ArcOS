
CC   ?= clang
LD   ?= ld
AR	 ?= ar
ASM  ?= nasm
QEMU ?= qemu-system-x86_64

CFLAGS += -g -I libc -I. -ffreestanding -Wall -Wextra -fno-exceptions -nobuiltininc -m32 -std=c11

OUT    = arcos.bin
BOOT   = boot/boot.bin
KERNEL = kernel/kernel.bin
LIBC   = libc/libc.a

KERNEL_SRC = $(shell find kernel/ -name '*.c')
KERNEL_OBJ = $(patsubst %.c,%.o,$(KERNEL_SRC))
KERNEL_OBJ += kernel/cpu/interrupts.o

LIBC_SRC = $(shell find libc/ -name '*.c')
LIBC_OBJ = $(patsubst %.c,%.o,$(LIBC_SRC))

all: $(OUT)

run: all
	"$(QEMU)" -d guest_errors -drive file=$(OUT),index=0,media=disk,format=raw

clean:
	rm -rf *.bin *.dis *.o *.elf $(OUT)
	rm -rf boot/*.o kernel/*.o

$(OUT): $(BOOT) $(KERNEL)
	cat $^ > $(OUT)

$(LIBC): $(LIBC_OBJ)
	$(AR) rcs $(LIBC) $(LIBC_OBJ)

$(KERNEL): boot/kernel_entry.o $(KERNEL_OBJ) $(LIBC)
	$(LD) -m elf_i386 -o $@.elf -Ttext 0x1000 $^ # for objdump
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $< -f elf -o $@

%.bin: %.asm
	$(ASM) $< -f bin -o $@
