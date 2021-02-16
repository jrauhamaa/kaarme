;;;
;;; empty_screen - fill screen with black
;;; no arguments
;;;
empty_screen:
    push es
    push di

    mov ax, VGA_SEGMENT
    mov es, ax
    xor ax, ax                  ; fill with zeroes
    mov cx, WIDTH_PIXELS*HEIGHT_PIXELS
    mov di, 0
    rep stosb

    pop di
    pop es
    ret


;;;
;;; draw_borders - draw borders of game area
;;; No arguments
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


;;;
;;; draw_snake_initial - draw snake in its initial location
;;; no arguments
;;;

draw_snake_initial:
    pusha
    mov bp, sp

    mov si, INITIAL_X
    mov di, INITIAL_Y
    xor bx, bx

    .init_square:
    push SNAKE_COLOR
    push di
    push si
    call draw_square

    inc bx
    add si, INITIAL_DIR_X
    add di, INITIAL_DIR_Y

    cmp bx, INITIAL_LENGTH
    jb .init_square

    mov sp, bp

    popa
    ret


;;;
;;; draw_square - draw square specified by coordinates
;;; arguments: x, y, color
;;;

draw_square:
    push bp
    mov bp, sp
    push es
    push di

    mov ax, VGA_SEGMENT
    mov es, ax

    mov cx, [bp+4]              ; x
    mov dx, [bp+6]              ; y
    mov ax, [bp+8]              ; al = pixel color

    imul dx, dx, WIDTH_PIXELS*SQUARE_HEIGHT
    imul cx, cx, SQUARE_WIDTH
    add dx, cx
    mov di, dx                  ; address of first pixel of square
                                ; (WIDTH_PIXELS*SQUARE_HEIGHT*y+SQUARE_WIDTH*x)

    mov cx, SQUARE_HEIGHT
.draw_row:
    push cx

    mov cx, SQUARE_WIDTH
    rep stosb

    pop cx
    add di, WIDTH_PIXELS-SQUARE_WIDTH

    loop .draw_row

    pop di
    pop es
    pop bp
    ret


