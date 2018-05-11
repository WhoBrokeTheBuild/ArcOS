[org 0x7c00]
[bits 16]

KERNEL_OFFSET   equ 0x1000

    mov [BOOT_DRIVE], dl

	mov bp, 0x9000			; Set the stack
	mov sp, bp

    call load_kernel_into_memory
	call switch_to_pm

	jmp $

load_kernel_into_memory:
	;; store all register values
	pusha

	;; set up parameters for disk_read routine
	mov bx, KERNEL_OFFSET
	mov dh, 128
	mov dl, [BOOT_DRIVE]
	call disk_read

	;; restore register values and ret
	popa
	ret

%include "boot/disk/disk.asm"
%include "boot/util/print_string.asm"
%include "boot/pm/gdt.asm"
%include "boot/pm/switch_to_pm.asm"
%include "boot/lm/detect_lm.asm"
%include "boot/lm/switch_to_lm.asm"

[bits 32]

pm_start:

    ;call detect_lm
    call execute_kernel
	jmp $

[bits 64]

lm_start:

    call execute_kernel
    jmp $

execute_kernel:
    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE      db 0

times 510-($-$$) db 0
dw 0xAA55
