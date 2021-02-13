;;;
;;; Draws borders of playing area
;;; No arguments
;;; Assumes we are in VGA mode
;;;

draw_borders:
    mov al, BORDER_COLOR
    mov dx, VGA_SEGMENT
    mov es, dx

    ; top border
    xor di, di
    mov cx, WIDTH_PIXELS
    rep stosb

    ; bottom border
    mov cx, WIDTH_PIXELS
    mov di, WIDTH_PIXELS * (HEIGHT_PIXELS - 1)
    rep stosb

    mov cx, HEIGHT_PIXELS
    xor di, di
.loop:
    mov byte [es:di], al                ; left border
    mov byte [es:di+WIDTH_PIXELS-1], al ; right border
    add di, WIDTH_PIXELS

    loop .loop

    ret



