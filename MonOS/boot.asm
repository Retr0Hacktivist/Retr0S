[BITS 16]
[ORG 0x7C00]

start:

    ; Charger le kernel
    mov bx, 0x7E00
    mov ah, 0x02
    mov al, 63
    mov ch, 0
    mov cl, 2
    mov dh, 0
    int 0x13

    ; Passer en mode protégé 32-bit
    cli                     ; désactiver les interruptions
    lgdt [gdt_descriptor]   ; charger la GDT
    mov eax, cr0
    or eax, 1
    mov cr0, eax            ; activer le mode protégé
    jmp 0x08:init_pm        ; saut far vers le segment code 32-bit

[BITS 32]
init_pm:
    mov ax, 0x10    ; segment data
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, 0x90000
    jmp 0x7E00      ; sauter au kernel

; ---- GDT ----
gdt_start:
    dq 0x0000000000000000   ; descripteur null
gdt_code:
    dw 0xFFFF, 0x0000
    db 0x00, 0x9A, 0xCF, 0x00
gdt_data:
    dw 0xFFFF, 0x0000
    db 0x00, 0x92, 0xCF, 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
