;;;
;;; to_text_mode - move to 80x25 text mode
;;;

to_text_mode:
    mov ah, 0x00
    mov al, 0x03
    int 10
    ret


;;;
;;; to_vga_mode - switch to 320x200 vga mode
;;;

to_vga_mode:
    mov ax, 0x0013
    int 0x10
    ret

;;;
;;; sleep - pause execution for a while
;;; arguments - upper 16 bits, lower 16 bits of delay (in microseconds)
;;;

;sleep:
    ;push bp
    ;mov bp, sp
;
    ;mov cx, word [bp+4]
    ;mov dx, word [bp+6]
    ;mov ah, 0x86
    ;int 15
;
    ;mov sp, bp
    ;pop bp
    ;ret


;;;
;;; register_interrupt_handler - register an interrupt handler
;;; arguments: interrupt number, code segment, procedure starting address
;;;
register_interrupt_handler:
    push bp
    mov bp, sp
    push es
    push di

    xor ax, ax
    mov es, ax                  ; interrupt vector is stored at segment 0

    mov di, word [bp+4]         ; interrupt number
    mov cx, word [bp+6]         ; code segment
    mov dx, word [bp+8]         ; procedure starting address

    shl di, 2                   ; multiply by 4 -> offset in interrupt vector
    mov [es:di], dx             ; a word for procedure address
    mov [es:di+2], cx           ; a word for code segment

    pop di
    pop es
    pop bp
    ret
