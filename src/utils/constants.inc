; segments
PART2_SEGMENT:      equ 0x0200
VGA_SEGMENT:        equ 0xa000

; bootloader part 2 sectors
PART2_START_SECTOR: equ 2
N_SECTORS_PART2:    equ 10

; vga dimensions
WIDTH_PIXELS:       equ 320
HEIGHT_PIXELS:      equ 200
SQUARE_WIDTH:       equ 10
SQUARE_HEIGHT:      equ 10
N_SQUARES_X:        equ WIDTH_PIXELS / SQUARE_WIDTH
N_SQUARES_Y:        equ HEIGHT_PIXELS / SQUARE_HEIGHT
N_SQUARES:          equ N_SQUARES_X*N_SQUARES_Y

; colors
COLOR_WHITE:        equ 0x0f
BORDER_COLOR:       equ COLOR_WHITE
SNAKE_COLOR:        equ COLOR_WHITE

; initial values
INITIAL_X:          equ 25
INITIAL_Y:          equ 10
INITIAL_DIR_X:      equ -1
INITIAL_DIR_Y:      equ 0
INITIAL_LENGTH:     equ 5

; game config
TURN_LENGTH:        equ 2
STATE_RUNNING:      equ 1
STATE_GAME_OVER:    equ 0

