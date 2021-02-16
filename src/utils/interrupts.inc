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


;;;
;;; get_keystroke - read keystroke from the buffer
;;; sets al as the ascii character & ah as the bios scancode
;;; or ax as 0 when no keystrokes
;;;

get_keystroke:
    xor ax, ax
    mov ah, 01h
    int 0x16                    ; test if a keystroke is in the buffer
    jc .empty

    xor ax, ax
    int 0x16                    ; read keystroke from the buffer
    ret

    .empty:
    xor ax, ax
    ret
