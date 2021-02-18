%define SNAKE_BUFFER_LENGTH

struc GameState
    .state:           resw 1
    .iteration:       resw 1
    .snake_tail:      resw 1    ; offset from snake_buffer beginning
    .snake_head:      resw 1    ; offset from snake_buffer beginning
    .snake_direction: resw 2    ; x & y
    .score:           resw 1
    .food_location:   resw 2    ; x & y
    .eating:          resw 1    ; 1 if eating this turn
    .turn_direction:  resw 1    ; direction in which to turn
endstruc

;------------------
;--- MAIN LOOP ----
;------------------
entry:
    call to_vga_mode
    call initialize

    push game_iteration
    push cs
    push 0x1c
    call register_interrupt_handler ; run game_iteration on each timer tick

main_loop:
    hlt

    call get_keystroke
    cmp ah, KEYCODE_LEFT
    je .left
    cmp ah, KEYCODE_RIGHT
    je .right
    cmp al, 'n'
    je .new_game

    jmp main_loop

    .left:
    mov word [cs:game_state + GameState.turn_direction], LEFT_TURN
    jmp main_loop

    .right:
    mov word [cs:game_state + GameState.turn_direction], RIGHT_TURN
    jmp main_loop

    .new_game:
    call initialize
    jmp main_loop


;;;
;;; initialize - set buffers & graphics to initial state
;;;

initialize:
    call init_game_state
    call init_snake_buffer
    call init_graphics
    ret


;;;
;;; game_iteration - advance game one iteration. run this routine periodically
;;;

game_iteration:
    cmp word [cs:game_state + GameState.state], STATE_GAME_OVER
    je .end

    dec word [cs:game_state + GameState.iteration]
    jnz .end
    add word [cs:game_state + GameState.iteration], TURN_LENGTH

    call turn_snake
    call advance_head
    call advance_tail
    call draw_score

    .end:
    iret


;;;
;;; to_game_over - move to game over state and draw game over screen
;;;

to_game_over:
    mov word [cs:game_state + GameState.state], STATE_GAME_OVER
    call empty_screen
    call draw_game_over_message
    ret


;;;
;;; turn_snake - update snake's direction
;;;

turn_snake:
    mov ax, word [cs:game_state + GameState.turn_direction]

    cmp ax, 0                   ; continue same dir
    je .end

    cmp ax, LEFT_TURN
    je .turn_left

    cmp ax, RIGHT_TURN
    je .turn_right

    .end:
    mov word [cs:game_state + GameState.turn_direction], 0
    ret

    .turn_left:                 ; dx = dy && dy = -dx
    mov cx, word [cs:game_state + GameState.snake_direction + 2]
    imul dx, word [cs:game_state + GameState.snake_direction], -1
    mov word [cs:game_state + GameState.snake_direction], cx
    mov word [cs:game_state + GameState.snake_direction + 2], dx

    jmp .end

    .turn_right:                ; dx = -dy && dy = dx
    imul cx, word [cs:game_state + GameState.snake_direction + 2], -1
    mov dx, word [cs:game_state + GameState.snake_direction]
    mov word [cs:game_state + GameState.snake_direction], cx
    mov word [cs:game_state + GameState.snake_direction + 2], dx

    jmp .end


;;;
;;; next_square - get next square for snake
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
;;;

advance_head:
    pusha
    mov bp, sp

    call next_square            ; move next square coordinates to ax, dx
    mov si, ax                  ; save coordinates to preserved registers
    mov di, dx

    push di
    push si
    call check_collisions
    cmp ax, 1                   ; ax = 1 -> collision
    je .game_over

    ; UPDATE GRAPHICS
    push SNAKE_COLOR
    push di
    push si
    call draw_square            ; draw snake head
    push word [cs:game_state + GameState.snake_direction + 2]   ; dy
    push word [cs:game_state + GameState.snake_direction]   ; dx
    push di
    push si
    call connect_squares

    ; UPDATE GAME STATE
    push word [cs:game_state + GameState.snake_head]
    call get_next_snake_buffer_index    ; move next snake buffer index to ax
    mov word [cs:game_state + GameState.snake_head], ax

    ; UPDATE SNAKE BUFFER
    imul bx, ax, 4
    mov word [cs:snake_buffer + bx], si
    mov word [cs:snake_buffer + bx + 2], di

    ; CHECK FOR FOOD
    cmp si, word [cs:game_state + GameState.food_location]
    jne .end
    cmp di, word [cs:game_state + GameState.food_location + 2]
    jne .end
    call eat_food

    .end:
    mov sp, bp
    popa
    ret

    .game_over:
    call to_game_over
    jmp .end


;;;
;;; advance_tail - advance snake tail, update graphics & buffers
;;;

advance_tail:
    pusha
    mov bp, sp

    cmp word [cs:game_state + GameState.eating], 1
    je .eating                  ; don't advance tail if eating

    ; UPDATE GRAPHICS
    imul bx, word [cs:game_state + GameState.snake_tail], 4 ; snake buffer offset
    mov si, word [cs:snake_buffer + bx]    ; old x coordinate
    mov di, word [cs:snake_buffer + bx + 2]    ; old y coordinate
    push di
    push si
    call empty_square

    ; UPDATE GAME STATE
    push word [cs:game_state + GameState.snake_tail]
    call get_next_snake_buffer_index    ; move next snake tail index to ax
    mov word [cs:game_state + GameState.snake_tail], ax

    .end:
    mov sp, bp
    popa
    ret

    .eating:
    mov word [cs:game_state + GameState.eating], 0
    jmp .end


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


;;;
;;; check_collisions - check if snake collides this turn
;;; arguments: x, y
;;; sets ax as 1 if collisions
;;;

check_collisions:
    push bp
    mov bp, sp
    push bx
    push di

    xor ax, ax
    mov cx, [cs:bp+4]              ; x
    mov dx, [cs:bp+6]              ; y
    mov bp, sp

    ; check borders
    cmp cx, 0                   ; collision with left border
    jl .collide
    cmp dx, MARGIN_TOP          ; collision with top border
    jl .collide
    cmp cx, N_SQUARES_X         ; collision with right border
    ja .collide
    cmp dx, N_SQUARES_Y         ; collision with bottom border
    ja .collide

    ; check if snake collides with itself
    mov di, word [cs:game_state + GameState.snake_tail]

    .loop
    imul bx, di, 4

    .check_x:
    cmp cx, word [cs:snake_buffer + bx]
    je .check_y
    jmp .loop_end

    .check_y:
    cmp dx, word [cs:snake_buffer + bx + 2]
    je .collide

    .loop_end:
    cmp di, word [cs:game_state + GameState.snake_head]
    je .end
    push di
    call get_next_snake_buffer_index
    mov di, ax
    xor ax, ax
    jmp .loop

    .collide:
    mov ax, 1

    .end:
    mov sp, bp
    pop di
    pop bx
    pop bp
    ret


eat_food:
    mov word [cs:game_state + GameState.eating], 1
    inc word [cs:game_state + GameState.score]
    call new_food
    ret

;;;
;;; new_food - create new food after eating
;;; TODO: calculate remainders properly
;;;

new_food:
    pusha
    mov bp, sp

    ; generate new coordinates for food
    .random_x:
    call pseudorandom
    cmp ax, 0
    je .random_y
    xor dx, dx
    mov cx, N_SQUARES_X
    mov si, dx

    .random_y:
    call pseudorandom
    cmp ax, 0
    je .collisions
    xor dx, dx
    mov cx, N_SQUARES_Y
    div cx
    mov di, dx

    ; if square occupied by snake, iterate through squares
    ; until empty one is found
    .collisions
    push di
    push si
    call check_collisions
    cmp ax, 1
    jne .end                    ; square not occupied by snake

    inc si                      ; x++
    cmp si, N_SQUARES_X
    jb .collisions

    xor si, si                  ; continue iterating from x=0 && y++
    inc di

    cmp di, N_SQUARES_Y
    jb .collisions

    xor di, di                  ; continue iterating from y=0
    jmp .collisions

    .end

    ; save coordinates
    mov word [cs:game_state + GameState.food_location], si
    mov word [cs:game_state + GameState.food_location + 2], di

    ; draw food
    push word FOOD_COLOR
    push di
    push si
    call draw_square

    mov sp, bp
    popa
    ret

;;;
;;; pseudorandom - generate a pseudorandom number.
;;;                Copied from c default implementation.
;;; the pseudorandom number is placed in ax
;;;

pseudorandom:
    imul ax, word [cs:.previous], 15245
    add ax, 12345
    mov word [cs:.previous], ax

    xor dx, dx
    mov cx, 32768
    div cx

    mov ax, dx                  ; remainder
    ret

    .previous: dw 1

;----------------
;--- IMPORTS ----
;----------------
%include "config"
%include "utils/constants.inc"
%include "utils/draw.inc"
%include "utils/initialize.inc"
%include "utils/interrupts.inc"
%include "utils/switch_video_mode.inc"

;-------------
;--- DATA ----
;-------------
game_over_msg: db "Game Over"
game_over_msg_len: equ $-game_over_msg

game_state:
istruc GameState
    at GameState.state,             dw STATE_RUNNING
    at GameState.iteration,         dw TURN_LENGTH
    at GameState.snake_tail,        dw 0
    at GameState.snake_head,        dw INITIAL_LENGTH - 1
    at GameState.snake_direction,   dw INITIAL_DIR_X, INITIAL_DIR_Y
    at GameState.score,             dw 0
    at GameState.food_location,     dw 0, 0
    at GameState.eating,            dw 0
    at GameState.turn_direction,    dw 0
iend


snake_buffer:  times 2*N_SQUARES dw 0

    times N_SECTORS_PART2*512-($-$$) db 0       ; pad sector with zeroes
