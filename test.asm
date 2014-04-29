	BITS 16

jmp start

%include "shutdown.asm"



	pause_msg db 'Press any key to continue...', 10, 13, 0


start:
	mov ax, 07C0h		; Set up 4K stack space after this bootloader
	add ax, 288		; (4096 + 512) / 16 bytes per paragraph
	mov ss, ax
	mov sp, 4096

	mov ax, 07C0h		; Set data segment to where we're loaded
	mov ds, ax

	

	call shutdown

	jmp $			; Jump here - infinite loop!


	
;
;  Display a message and wait for user input
;
pause:

	mov si, pause_msg
	call print_string

	mov ah, 00h		; Wait for user input
	int 16h

	ret

;
;  Print a string stored in [si]
;
print_string:			; Routine: output string in \SI to screen
	mov ah, 0Eh		; int 10h 'print char' function

.repeat:
	lodsb			; Get character from string
	cmp al, 0
	je .done		; If char is zero, end of string
	int 10h			; Otherwise, print it
	jmp .repeat

.done:
	ret


; Finish boot sector

	times 510-($-$$) db 0	; Pad remainder of boot sector with 0s
	dw 0xAA55		; The standard PC boot signature
