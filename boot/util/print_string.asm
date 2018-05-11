[bits 16]

;
;  Print a string stored in [bx]
;
print_string_16:
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

[bits 32]

VIDEO_MEMORY 	equ 0xB8000
WHITE_ON_BLACK	equ 0x4F

print_string_32:
	pusha
	mov edx, VIDEO_MEMORY		; Set EDX to the start of Video Memory

.repeat:
	mov al, [ebx]				; Store the char at EBX in AL
	mov ah, WHITE_ON_BLACK		; Store the attributs in AH

	cmp al, 0					; End of string?
	je .done

	mov [edx], ax				; Store char and attributes at current character cell

	add ebx, 1					; Increment EBX to the next char in the string
	add edx, 2					; Move to next character cell in Video Memory

	jmp .repeat

.done:
	popa
	ret
