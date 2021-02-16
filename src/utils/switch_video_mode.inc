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

