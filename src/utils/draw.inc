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
    push word SNAKE_COLOR
    push di
    push si
    call draw_square

    push word INITIAL_DIR_Y
    push word INITIAL_DIR_X
    push di
    push si
    call connect_squares

    inc bx
    add si, INITIAL_DIR_X
    add di, INITIAL_DIR_Y

    cmp bx, INITIAL_LENGTH
    jb .init_square

    ; erase tail expanding to previous square
    push word INITIAL_Y - INITIAL_DIR_Y
    push word INITIAL_X - INITIAL_DIR_X
    call empty_square

    mov sp, bp

    popa
    ret


;;;
;;; empty_square - fill square with black without erasing borders
;;; arguments: x, y
;;;

empty_square:
    push bp
    mov bp, sp
    push si
    push di

    mov ax, SQUARE_WIDTH
    imul ax, word [bp+4]
    mov dx, SQUARE_HEIGHT
    imul dx, word [bp+6]
    mov si, SQUARE_WIDTH
    mov di, SQUARE_HEIGHT

    cmp word [bp+4], 0
    jne .cont1
    mov ax, 2
    sub si, 2
    .cont1:

    cmp word [bp+6], 0
    jne .cont2
    mov dx, 2
    sub di, 2
    .cont2:

    cmp word [bp+4], N_SQUARES_X - 1
    jne .cont3
    sub si, 2
    .cont3:

    cmp word [bp+6], N_SQUARES_Y - 1
    jne .cont4
    sub di, 2
    .cont4:

    mov bp, sp
    push word 0                 ; fill with zeroes
    push di                     ; height
    push si                     ; width
    push dx                     ; y
    push ax                     ; x
    call draw_rect

    mov sp, bp
    pop di
    pop si
    pop bp
    ret


;;;
;;; connect_squares - fill padding between squares
;;; arguments: x, y, dx, dy
;;;

connect_squares:
    push bp
    mov bp, sp

    mov ax, SQUARE_WIDTH
    imul ax, word [bp+4]
    add ax, SQUARE_PADDING
    mov dx, SQUARE_HEIGHT
    imul dx, word [bp+6]
    add dx, SQUARE_PADDING

    cmp word [bp+8], 0
    jne .horizontal
    jmp .vertical

    .end:
    push SNAKE_COLOR
    push SQUARE_HEIGHT - 2*SQUARE_PADDING
    push SQUARE_WIDTH - 2*SQUARE_PADDING
    push dx                     ; y
    push ax                     ; x
    call draw_rect

    mov sp, bp
    pop bp
    ret

    .horizontal:
    mov cx, SQUARE_WIDTH/2
    imul cx, word [bp+8]
    sub ax, cx

    jmp .end

    .vertical:
    mov cx, SQUARE_HEIGHT/2
    imul cx, word [bp+10]
    sub dx, cx

    jmp .end


;;;
;;; draw_square - draw square specified by coordinates
;;; arguments: x, y, color
;;;

draw_square:
    push bp
    mov bp, sp

    mov ax, SQUARE_WIDTH
    imul ax, [bp+4]
    add ax, SQUARE_PADDING
    mov dx, SQUARE_HEIGHT
    imul dx, [bp+6]
    add dx, SQUARE_PADDING

    push word [bp+8]            ; color
    push word SQUARE_HEIGHT-2*SQUARE_PADDING
    push word SQUARE_WIDTH-2*SQUARE_PADDING
    push dx                     ; y
    push ax                     ; x
    call draw_rect

    mov sp, bp
    pop bp
    ret


;;;
;;; draw_rect - draw a rectangle
;;; arguments - x, y, width, height, color
;;;

draw_rect:
    push bp
    mov bp, sp
    push bx
    push di

    mov ax, VGA_SEGMENT
    mov es, ax
    mov di, WIDTH_PIXELS
    imul di, word [bp+6]
    add di, [bp+4]              ; memory offset -> di
    mov bx, di                  ; keep di copy in bx
    mov ax, word [bp+12]        ; color

    mov cx, word [bp+10]        ; height
    .draw_row:
    push cx
    mov cx, word [bp+8]         ; width
    rep stosb
    pop cx

    add bx, WIDTH_PIXELS
    mov di, bx

    loop .draw_row

    pop di
    pop bx
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
