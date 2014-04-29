[bits 16]

;
;  Display a message and wait for user input
;
pause:

	mov si, pause_msg
	call print_string

	mov ah, 00h		; Wait for user input
	int 16h

	ret
