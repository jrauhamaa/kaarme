%define SNAKE_BUFFER_LENGTH

struc GameState
    .iteration: resw 1
    .snake_tail:  resw 1        ; offset from snake_buffer beginning
    .snake_head:    resw 1      ; offset from snake_buffer beginning
    .snake_direction: resw 2    ; x & y
    .score:         resw 1
    .food_location  resw 2      ; x & y
endstruc

main:
    call to_vga_mode
    call initialize

    push game_iteration
    push cs
    push 0x1c
    call register_interrupt_handler


.end:
    hlt
    jmp .end

initialize:
    call init_snake_buffer
    call init_grid_buffer
    call init_graphics
    ret

game_iteration:

    dec word [cs:game_state + GameState.iteration]
    jnz .end

    add word [cs:game_state + GameState.iteration], TURN_LENGTH
    call advance_head
    call advance_tail

    .end:
    iret

;;;
;;; next_square - get next square for snake
;;; no arguments
;;; returns x and y coordinates (in ax and dx, respectively)
;;;

next_square:
    push bx
    push si
    push di

    imul bx, [cs:game_state + GameState.snake_head], 4
    add bx, snake_buffer
    mov ax, word [cs:bx]           ; snake head x
    mov dx, word [cs:bx+2]         ; snake head y
    mov si, word [cs:game_state + GameState.snake_direction]     ; x direction
    mov di, word [cs:game_state + GameState.snake_direction + 2] ; y direction
    add ax, si                  ; next square x
    add dx, di                  ; next square y

    pop di
    pop si
    pop bx
    ret

;;;
;;; advance_head - advance snake head, update graphics & buffers
;;; no arguments
;;;

advance_head:
    pusha
    mov bp, sp

    call next_square            ; move next square coordinates to ax, dx
    mov si, ax                  ; save coordinates to preserved registers
    mov di, dx

    ; UPDATE GRAPHICS
    push SNAKE_COLOR
    push dx
    push ax
    call draw_square            ; draw snake head

    ; UPDATE GAME STATE
    push word [cs:game_state + GameState.snake_head]
    call get_next_snake_buffer_index    ; move next snake buffer index to ax
    mov word [cs:game_state + GameState.snake_head], ax

    ; UPDATE SNAKE BUFFER
    imul bx, ax, 4
    mov word [cs:snake_buffer + bx], si
    mov word [cs:snake_buffer + bx + 2], di

    ; UPDATE GRID BUFFER
    mov bx, 2
    imul bx, si
    imul bx, di                 ; move grid buffer offset to ax
    mov word [cs:grid_buffer + bx], 1

    mov sp, bp
    popa
    ret

;;;
;;; advance_tail - advance snake tail, update graphics & buffers
;;; no arguments
;;;

advance_tail:
    pusha
    mov bp, sp

    ; UPDATE GRAPHICS
    imul bx, word [cs:game_state + GameState.snake_tail], 4 ; snake buffer offset
    mov si, word [cs:snake_buffer + bx]    ; old x coordinate
    mov di, word [cs:snake_buffer + bx + 2]    ; old y coordinate
    push word 0                 ; empty square color
    push di
    push si
    call draw_square

    ; UPDATE GRID BUFFER
    mov bx, 2
    imul bx, si
    imul bx, di                 ; grid buffer offset -> bx
    mov word [cs:grid_buffer + bx], 0

    ; UPDATE GAME STATE
    push word [cs:game_state + GameState.snake_tail]
    call get_next_snake_buffer_index    ; move next snake tail index to ax
    mov word [cs:game_state + GameState.snake_tail], ax

    mov sp, bp
    popa
    ret

;;;
;;; next_snake_buffer_index - return next index in snake buffer
;;; 1 argument: previous index
;;; return value is placed in ax
;;;

get_next_snake_buffer_index:
    push bp
    mov bp, sp

    mov ax, word [bp+4]
    inc ax
    cmp ax, N_SQUARES
    jb .end
    sub ax, N_SQUARES
    .end:

    pop bp
    ret

;----------------
;--- IMPORTS ----
;----------------
%include "utils/constants.inc"
%include "utils/draw.inc"
%include "utils/initialize.inc"
%include "utils/interrupts.inc"

;-------------
;--- DATA ----
;-------------
game_state:
istruc GameState
    at GameState.iteration,         dw TURN_LENGTH
    at GameState.snake_tail,        dw 0
    at GameState.snake_head,        dw INITIAL_LENGTH - 1
    at GameState.snake_direction,   dw INITIAL_DIR_X, INITIAL_DIR_Y
    at GameState.score,             dw 0
    at GameState.food_location,     dw 0, 0
iend


snake_buffer:  times 2*N_SQUARES dw 0
grid_buffer:   times N_SQUARES dw 0

    times N_SECTORS_PART2*512-($-$$) db 0       ; pad sector with zeroes
