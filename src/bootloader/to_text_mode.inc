;;;
;;; clears screen and moves to 80x25 text mode
;;;

to_text_mode:
    mov ah, 0x00
    mov al, 0x03
    int 10
    ret
