
; Strings

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
