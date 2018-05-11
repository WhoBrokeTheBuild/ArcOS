[bits 32]

detect_lm:
    ;; Copy FLAGS to EAX
    pushfd
    pop eax

    ;; Store copy in ECX
    mov ecx, eax

    ;; Flip the ID bit
    xor eax, 1 << 21

    ;; Copy EAX to FLAGS
    push eax
    popfd

    ;; Copy FLAGS back to EAX
    pushfd
    pop eax

    ;; Restore FLAGS from copy in ECX
    push ecx
    popfd

    ;; See if ID bit was flipped
    xor eax, ecx
    jz .no_long_mode

    ;; Check if long mode is available
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode

    call switch_to_lm
    ret

.no_long_mode:
    ;; No Long Mode
    call execute_kernel
    jmp $
