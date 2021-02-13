;;;
;;; Switch to 320x200 video mode
;;;

to_video_mode:
    mov ax, 0x0013
    int 0x10
    ret
