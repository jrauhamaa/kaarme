; segments
PART2_SEGMENT:      equ 0x0200
VGA_SEGMENT:        equ 0xa000

; bootloader part 2 sectors
PART2_START_SECTOR: equ 2
N_SECTORS_PART2:    equ 11

; vga dimensions
WIDTH_PIXELS:       equ 320
HEIGHT_PIXELS:      equ 200
N_SQUARES_X:        equ WIDTH_PIXELS / SQUARE_WIDTH
N_SQUARES_Y:        equ HEIGHT_PIXELS / SQUARE_HEIGHT
N_SQUARES:          equ N_SQUARES_X*N_SQUARES_Y

; colors
COLOR_WHITE:        equ 0x0f
COLOR_RED:          equ 0x04

; keycodes
KEYCODE_LEFT:       equ 0x4b
KEYCODE_RIGHT:      equ 0x4d

; initial values
INITIAL_X:          equ N_SQUARES_X-2
INITIAL_Y:          equ N_SQUARES_Y/2
INITIAL_DIR_X:      equ -1
INITIAL_DIR_Y:      equ 0

; game config
STATE_RUNNING:      equ 1
STATE_GAME_OVER:    equ 0
LEFT_TURN:          equ -1
RIGHT_TURN:         equ 1
