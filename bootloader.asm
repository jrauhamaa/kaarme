bits 16
[org 0x7c00]

main:
    push dx
    call to_text_mode
    pop dx
    call print_registers

.end:
    cli
    hlt
    jmp .end


.message: db "joujou!", 0x0a, 0x0d, 0

%include "to_text_mode.inc"
%include "print_string.inc"
%include "print_hex.inc"
%include "print_registers.inc"

    times 510-($-$$) db 0
    dw 0xaa55
