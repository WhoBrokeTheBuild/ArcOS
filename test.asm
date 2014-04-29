[org 0x7c00]

	mov bp, 0x9000			; Set the stack
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print_string
    
	call switch_to_pm

	jmp $
	
%include "util16/print_string.asm"
%include "gdt.asm"
%include "util32/print_string_pm.asm"
%include "switch_to_pm.asm"

[bits 32]

pm_start:

	mov ebx, MSG_PROT_MODE
	call print_string_pm

	jmp $				; Hang

MSG_REAL_MODE	db "Started in 16-bit Real Mode", 0
MSG_ABOUT_TO_SWITCH	db "About to switch", 0
MSG_PROT_MODE	db "Successfully landed in 32-bit Protected Mode", 0

times 510-($-$$) db 0
dw 0xAA55
