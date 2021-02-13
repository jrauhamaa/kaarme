;;;
;;; C-style function.
;;; 1 argument - the beginning address of (null-terminated) string to print
;;; Expects BIOS to be in text mode (mov ax, 0x0003 & int 0x10)
;;;

print_string:
    push bp
    mov bp, sp
    push si
    mov si, [bp+4]              ; store address of string in si

    mov ah, 0x0e                ; print char interrupt

.print_char:
    lodsb
    cmp al, 0                   ; if character is NULL, return
    je .end

    int 0x10                    ; print character
    jmp .print_char

.end:
    pop si
    pop bp
    ret
