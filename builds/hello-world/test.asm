	BITS 16

start:
	mov ax, 07C0h		; Set up 4K stack space after this bootloader
	add ax, 288		; (4096 + 512) / 16 bytes per paragraph
	mov ss, ax
	mov sp, 4096

	mov ax, 07C0h		; Set data segment to where we're loaded
	mov ds, ax

	mov si, text_string	; Put string position into SI
	call print_string	; Call our string-printing routine

	call pause

	call shutdown

	jmp $			; Jump here - infinite loop!


	
	; Strings

	text_string db 'Hello, World!', 10, 13, 0

	pause_msg db 'Press any key to continue...', 10, 13, 0

	shutdown_error_msg db 'Shutdown Failed', 10, 13, 0

	check_apm_success_msg db 'APM Check Succeeded', 10, 13, 0
	check_apm_error_msg db 'APM Check Failed', 10, 13, 0

	init_apm_success_msg db 'APM Init Succeeded', 10, 13, 0
	init_apm_error_msg db 'APM Init Failed', 10, 13, 0

	term_apm_success_msg db 'APM Term Succeeded', 10, 13, 0
	term_apm_error_msg db 'APM Term Failed', 10, 13, 0

	apm_shutdown_success_msg db 'APM Shutdown Succeeded', 10, 13, 0
	apm_shutdown_error_msg db 'APM Shutdown Failed', 10, 13, 0

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

;
;  Checks if APM is available, and if so uses it to shutdown
;
shutdown:
	
	call check_APM
	jc .shutdown_error

	call init_APM
	jc .shutdown_error

	call APM_shutdown
	jc .shutdown_error

	call term_APM
	jc .shutdown_error

.shutdown_success:

	ret

.shutdown_error:
	
	mov si, shutdown_error_msg
	call print_string

	ret


;
;  Checks if APM is available
;
check_APM:

	mov ah, 53h
	mov al, 00h
	xor bx, bx
	int 15h
	jc .check_APM_error

.check_APM_succes:
	
	mov si, check_apm_success_msg
	call print_string

	ret

.check_APM_error:

	mov si, check_apm_error_msg
	call print_string

	stc
	ret

;
;  Connect to the APM interface
;
init_APM:

	mov ah, 53h
	mov al, 01h ; 01h = Real Mode Interface
	xor bx, bx
	int 15h
	jc .init_APM_error

.init_APM_success:

	mov si, init_apm_success_msg
	call print_string

	ret

.init_APM_error:

	mov si, init_apm_error_msg
	call print_string

	stc
	ret

;
;  Terminate the connection to the APM interface
;
term_APM:

	mov ah, 53h
	mov al, 04h
	xor bx, bx
	int 15h
	jc .term_APM_error

.term_APM_success:

	mov si, term_apm_success_msg
	call print_string

	ret

.term_APM_error:

	mov si, term_apm_error_msg
	call print_string

	stc
	ret

;
;  Sets the APM Power State to Off
;
APM_shutdown:

	mov ah, 53h
	mov al, 07h
	mov bx, 0001h
	mov cx, 03h 	; Set power state to Off
	int 15h
	jc .APM_shutdown_error

.APM_shutdown_success:

	mov si, apm_shutdown_success_msg
	call print_string

	ret

.APM_shutdown_error:

	mov si, apm_shutdown_error_msg
	call print_string

	ret

	; Finish boot sector

	times 510-($-$$) db 0	; Pad remainder of boot sector with 0s
	dw 0xAA55		; The standard PC boot signature
