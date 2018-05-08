
;
;  Checks if APM is available, and if so uses it to shutdown
;
shutdown:
	call init_APM
	jc .shutdown_error

	call APM_shutdown
	jc .shutdown_error

	call term_APM
	jc .shutdown_error

.shutdown_success:
	ret

.shutdown_error:
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
	ret

.init_APM_error:
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
	ret

.term_APM_error:
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
	ret

.APM_shutdown_error:
	ret
