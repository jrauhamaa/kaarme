bits 16
[org 0x7c00]

main:
    mov byte [.drive_number], dl ; boot drive number is stored in dl

    call to_text_mode

.load_part2:
    mov ax, PART2_SEGMENT
    mov es, ax                  ; load to PART2_SEGMENT
    xor bx, bx                  ; segment offset = 0
    mov dh, 0                   ; head 0
    mov dl, [.drive_number]
    mov ch, 0                   ; cylinder 0
    mov cl, PART2_START_SECTOR

    mov ah, 0x02                ; BIOS int 13&ah=2 = read disk sectors into mem
    mov al, N_SECTORS_PART2

    int 0x13

    jc .error

    mov ax, PART2_SEGMENT
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp PART2_SEGMENT:0x00

.error:
    push .error_message
    call print_string

.end:
    cli
    hlt
    jmp .end


.error_message: db "Error loading disk", 0
.drive_number: db 0

%include "config"
%include "utils/switch_video_mode.inc"
%include "utils/constants.inc"
%include "utils/print.inc"

    times 510-($-$$) db 0
    dw 0xaa55
