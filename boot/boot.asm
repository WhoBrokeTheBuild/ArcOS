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
	mov dh, 5
	mov dl, [BOOT_DRIVE]
	call disk_read

	;; restore register values and ret
	popa
	ret

%include "boot/disk/disk.asm"
%include "boot/pm/gdt.asm"
%include "boot/pm/switch_to_pm.asm"
%include "boot/util/print_string.asm"

[bits 32]

pm_start:

    mov ebx, MSG_PROT_MODE
    call print_string

    call KERNEL_OFFSET
	jmp $				; Hang

BOOT_DRIVE      db 0
MSG_PROT_MODE	db "Successfully landed in 32-bit Protected Mode", 0

times 510-($-$$) db 0
dw 0xAA55
