;;;
;;; C-style function.
;;; 1 argument - the beginning address of (null-terminated) string to print
;;; Expects BIOS to be in text mode (mov ax, 0x0003 & int 0x10)
;;;

print_string:
    push bp
    mov bp, sp
    push bx
    mov bx, [bp+4]              ; store address of string in bx

    mov ah, 0x0e                ; print char interrupt

.print_char:
    mov al, [bx]                ; character to print
    cmp al, 0                   ; if character is NULL, return
    je .end

    int 0x10                    ; print character
    inc bx                      ; next char
    jmp .print_char

.end:
    pop bx
    pop bp
    ret
