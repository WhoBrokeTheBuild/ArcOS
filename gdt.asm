; GDT
gdt_start:

gdt_null:
	dd 0x0			; Mandatory Null Descriptor
	dd 0x0

gdt_code:
	; Base = 0x0
	; Limit = 0xfffff
	; 1st Flags: (present)1 (priviledge)00 (descriptor type)1   	    -> 1001b
	; Type Flags: (code)1 (conforming)0 (readable)1 (accessed)0 	    -> 1010b
	; 2nd Flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0  -> 1100b
	dw 0xffff		; Limit (bits 0-15)
	dw 0x0			; Base (bits 0-15)
	db 0x0			; Base (bits 16-23)
	db 10011010b	; 1st Flags, Type Flags
	db 11001111b	; 2nd Flags, Limit (bits 16-19)
	db 0x0			; Base (bits 24-31)

gdt_data:
	dw 0xffff		; Limit (bits 0-15)
	dw 0x0			; Base (bits 0-15)
	db 0x0			; Base (bits 16-23)
	db 10010010b	; 1st Flags, type flags
	db 11001111b	; 2nd Flags, Limit (bits 16-19)
	db 0x0			; Base (bits 24-31)

gdt_end:


gdt_descriptor:
	dw gdt_end - gdt_start - 1	; Size of the GDT, always one less
	dd gdt_start				; Start address of the GDT

GDT_CODE_SEG equ gdt_code - gdt_start
GDT_DATA_SEG equ gdt_data - gdt_start
