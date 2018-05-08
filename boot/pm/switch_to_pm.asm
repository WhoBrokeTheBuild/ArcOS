[bits 16]

;
;  Switch to protected mode
;
switch_to_pm:
	cli

	lgdt [gdt_descriptor]

	mov eax, cr0 				; Set the first bit of CR0, a control register
	or eax, 0x1
	mov cr0, eax

	jmp GDT_CODE_SEG:init_pm	; Make a far jump to the 32-bit code. This
	                            ; forces the CPU to flush its cache of
	                            ; pre-fetched and real-mode decoded instructions
	                            ; which can cause problems

[bits 32]

;
;  Initialize registers and the stack once in PM
;
init_pm:
	mov ax, GDT_DATA_SEG		; Now in PM, our old segments are meaningless,
    mov ds, ax                  ; so we point the segment registers to the data
	mov ss, ax                  ; selector from the GDT
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp

	call pm_start
