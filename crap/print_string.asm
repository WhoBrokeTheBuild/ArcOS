[bits 16]

;
;  Print a string stored in [bx]
;
print_string:
	mov ah, 0Eh
	mov si, bx

.repeat:
	lodsb			; Get character from string
	cmp al, 0
	je .done		; If char is zero, end of string
	int 10h			; Otherwise, print it
	jmp .repeat

.done:
	ret
