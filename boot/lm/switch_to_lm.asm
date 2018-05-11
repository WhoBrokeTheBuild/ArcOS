[bits 32]

switch_to_lm:
    ;; Disable 32-bit paging
    mov eax, cr0
    and eax, 01111111111111111111111111111111b
    mov cr0, eax

    ;; Clear old tables
    mov edi, 0x1000
    mov cr3, edi
    xor eax, eax
    mov ecx, 4096
    rep stosd
    mov edi, cr3

    ;; Set new tables
    mov DWORD [edi], 0x2003
    add edi, 0x1000
    mov DWORD [edi], 0x3003
    add edi, 0x1000
    mov DWORD [edi], 0x4003
    add edi, 0x1000
    mov ebx, 0x00000003
    mov ecx, 512

;; Set entries
.set_entry:

    mov DWORD [edi], ebx
    add ebx, 0x1000
    add edi, 8
    loop .set_entry

;; Enable PAE
.enable_paging:

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ;; Set the LM bit
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    lgdt [gdt_descriptor]

    ;; Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ;; Load our GDT with the 64-bit flags
    jmp GDT_CODE_SEG:init_lm

[bits 64]

init_lm:

    ;; Update segment registers
    mov ax, GDT_DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ;; Update base and stack pointers
    mov ebp, 0x90000
    mov esp, ebp

    call lm_start
