;;;
;;; init_snake_buffer - fill snake buffer with initial values
;;;

init_snake_buffer:
    pusha

    mov bx, snake_buffer
    mov si, INITIAL_X
    mov di, INITIAL_Y
    mov cx, INITIAL_LENGTH

    .snake_buffer_item:
    mov word [bx], si
    mov word [bx+2], di

    add bx, 4
    add si, INITIAL_DIR_X
    add di, INITIAL_DIR_Y

    loop .snake_buffer_item

    popa
    ret


;;;
;;; init_graphics - draw snake in its initial location
;;;

init_graphics:
    call empty_screen
    call draw_borders
    call draw_snake_initial
    call new_food
    ret


;;;
;;; init_game_state - fill game_state-struct with initial values
;;;

init_game_state:
    mov word [cs:game_state + GameState.state], STATE_RUNNING
    mov word [cs:game_state + GameState.iteration], TURN_LENGTH
    mov word [cs:game_state + GameState.snake_tail], 0
    mov word [cs:game_state + GameState.snake_head], INITIAL_LENGTH-1
    mov word [cs:game_state + GameState.snake_direction], INITIAL_DIR_X
    mov word [cs:game_state + GameState.snake_direction + 2], INITIAL_DIR_Y
    mov word [cs:game_state + GameState.score], 0
    mov word [cs:game_state + GameState.food_location], 0
    mov word [cs:game_state + GameState.food_location + 2], 0
    mov word [cs:game_state + GameState.eating], 0
    mov word [cs:game_state + GameState.turn_direction], 0
    ret
