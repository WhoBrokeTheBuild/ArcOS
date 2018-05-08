; GDT
gdt_start:

gdt_null:
	dd 0x0			; Mandatory Null Descriptor
	dd 0x0

gdt_kernel_code:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10011010b
	db 11001111b
	db 0x0

gdt_kernel_data:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0

gdt_userland_code:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 11111010b
	db 11001111b
	db 0x0

gdt_userland_data:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 11110010b
	db 11001111b
	db 0x0

gdt_end:
gdt_descriptor:
	dw gdt_end - gdt_start - 1	; Size of the GDT, always one less
	dd gdt_start				; Start address of the GDT

GDT_CODE_SEG equ gdt_kernel_code - gdt_start
GDT_DATA_SEG equ gdt_kernel_data - gdt_start
