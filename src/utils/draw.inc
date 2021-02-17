;;;
;;; empty_screen - fill screen with black
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
;;;

draw_borders:
    pusha

    mov al, BORDER_COLOR
    mov dx, VGA_SEGMENT
    mov es, dx

    ; top border
    mov di, WIDTH_PIXELS*SQUARE_HEIGHT
    mov cx, WIDTH_PIXELS
    rep stosb

    ; bottom border
    mov cx, WIDTH_PIXELS
    mov di, WIDTH_PIXELS * (HEIGHT_PIXELS - 1)
    rep stosb

    ;mov cx, HEIGHT_PIXELS
    ;xor di, di
    ;mov di, WIDTH_PIXELS*SQUARE_HEIGHT
    mov cx, HEIGHT_PIXELS - SQUARE_HEIGHT
    mov di, SQUARE_HEIGHT*MARGIN_TOP
    .loop:
    mov bx, WIDTH_PIXELS
    imul bx, di
    mov byte [es:bx], al
    mov byte [es:bx+WIDTH_PIXELS-1], al
    inc di

    ;mov byte [es:di], al                ; left border
    ;mov byte [es:di+WIDTH_PIXELS-1], al ; right border
    ;add di, WIDTH_PIXELS

    loop .loop

    popa
    ret


;;;
;;; draw_snake_initial - draw snake in its initial location
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


;;;
;;; draw_game_over_message - draw game over text in the middle of the screen
;;;

draw_game_over_message:
    pusha

    mov ax, cs
    mov es, ax                  ; message segment
    xor ax, ax
    mov bp, game_over_msg       ; message address
    mov cx, game_over_msg_len   ; message length
    mov ah, 0x13                ; write string interrupt
    mov bx, 0x0064
    mov dl, 10                  ; column
    mov dh, 10                  ; row
    int 0x10

    popa
    ret


draw_score:
    pusha

    ; zero-initialize .score_text
    mov ax, cs
    mov es, ax
    mov di, .score_text
    mov cx, .score_text_len
    mov al, '0'
    rep stosb

    mov bx, 10
    mov cx, .score_text_len
    mov di, .score_text + .score_text_len - 1
    mov ax, [cs:game_state + GameState.score]

    .loop:

    cmp ax, 0                   ; division of zero results in error
    je .draw

    div bx                      ; quotient - ax, remainder - dx
    add dx, '0'
    mov [cs:di], dl
    dec di
    jmp .loop

    .draw:

    xor ax, ax
    mov bp, .score_text
    mov cx, .score_text_len
    mov ah, 0x13
    mov bx, 0x0064
    mov dl, 0                   ; column
    mov dh, 0                   ; row
    int 0x10

    popa
    ret

    .score_text: db "00000"
    .score_text_len: equ $-.score_text
