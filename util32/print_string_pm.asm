[bits 32]

VIDEO_MEMORY 	equ 0xB80000
WHITE_ON_BLACK	equ 0x0F

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY		; Set EDX to the start of Video Memory

print_string_pm_loop:
	mov al, [ebx]				; Store the char at EBX in AL
	mov ah, WHITE_ON_BLACK		; Store the attributs in AH

	cmp al, 0					; End of string?
	je print_string_pm_done

	mov [edx], ax				; Store char and attributes at current character cell

	add ebx, 1					; Increment EBX to the next char in the string
	add edx, 2					; Move to next character cell in Video Memory

	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret
