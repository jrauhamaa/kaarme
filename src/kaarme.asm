main:
    call to_video_mode
    call draw_borders


.end:
    jmp $


.message: db "You're now in part 2 of the bootloader!", 0x0a, 0x0d, 0
.end_message: db "Halting cpu", 0x0a, 0x0d, 0

%include "constants.inc"
%include "kaarme/to_video_mode.inc"
%include "kaarme/draw_borders.inc"

    times N_SECTORS_PART2*512-($-$$) db 0       ; pad sector with zeroes
