	BITS 16

jmp start

%include "shutdown-slim.asm"

	startup_msg 	db 'Welcome to ArcOS', 10, 13, 0

	pause_msg 	db 'Press any key to continue...', 10, 13, 0

	prompt_msg 	db '>', 0

	err_bad_cmd_msg db 'Error: Bad Command Entered', 10, 13, 0

	cmd_hi 		db 'hi', 0
	cmd_help 	db 'help', 0
	cmd_exit	db 'exit', 0

	hi_msg		db 'Hello, World!', 10, 13, 0
	
	help_msg	db 'Commands: hi, help, exit', 10, 13, 0

	buffer times 64 db 0

start:
	mov ax, 07C0h		; Set up 4K stack space after this bootloader
	add ax, 288		; (4096 + 512) / 16 bytes per paragraph
	mov ss, ax
	mov sp, 4096

	mov ax, 07C0h		; Set data segment to where we're loaded
	mov ds, ax
	mov es, ax

	mov si, startup_msg
	call print_string

	call prompt

	;call pause
	call shutdown

	jmp $			; Jump here - infinite loop!


;
;  Runs a prompt to read in a command
;
prompt:
	mov si, prompt_msg
	call print_string

	mov di, buffer
	call get_string

	mov si, buffer
	cmp byte[si], 0 	; Blank line?
	je prompt		; Ignore it

	mov si, buffer
	mov di, cmd_hi		; "hi" command
	call strcmp
	jc .helloworld

	mov si, buffer
	mov di, cmd_help	; "help" command
	call strcmp
	jc .help

	mov si, buffer
	mov di, cmd_exit	; "exit" command
	call strcmp
	jc .exit

	mov si, err_bad_cmd_msg
	call print_string
	jmp prompt

.helloworld:
	mov si, hi_msg
	call print_string

	jmp prompt

.help:
	mov si, help_msg
	call print_string

	jmp prompt

.exit:
	ret
	
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
print_string:
	mov ah, 0Eh

.repeat:
	lodsb			; Get character from string
	cmp al, 0
	je .done		; If char is zero, end of string
	int 10h			; Otherwise, print it
	jmp .repeat

.done:
	ret

;
;  Read a string and store it in a buffer
;
get_string:
	xor cl, cl

.loop:
	mov ah, 0
	int 0x16

	cmp al, 0x08 	; Backspace?
	je .backspace

	cmp al, 0x0D 	; Enter?
	je .done

	cmp cl, 0x3F 	; 63 chars entered?
	je .loop

	mov ah, 0x0E
	int 0x10	; Print the character

	stosb		; Put character in buffer
	inc cl
	jmp .loop

.backspace:
	cmp cl, 0	; Empty buffer?
	je .loop

	dec di
	mov byte[di], 0	; Delete Character
	dec cl		; Decrement counter

	mov ah, 0x0E
	mov al, 0x08
	int 10h		; Backspace on screen

	mov al, ' '
	int 10h		; Blank character out

	mov al, 0x08	; Backspace again
	int 10h

	jmp .loop

.done:
	mov al, 0  	; Add null terminator
	stosb

	mov ah, 0x0E
	mov al, 0x0D
	int 10h
	mov al, 0x0A
	int 10h		; Newline

	ret

;
;  String comparison between [si] and [di]
;
strcmp:
.loop:
	mov al, [si]	; Grab a byte from SI
	mov bl, [di]	; Grab a byte from DI
	cmp al, bl
	jne .notequal
	
	cmp al, 0 	; Are both bytes null?
	je .done 	; Yes, we're done

	inc di
	inc si
	jmp .loop

.notequal:
	clc
	ret

.done
	stc
	ret

; Finish boot sector

	times 510-($-$$) db 0	; Pad remainder of boot sector with 0s
	dw 0xAA55		; The standard PC boot signature
