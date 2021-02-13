;;;
;;; print contents of all registers
;;; no arguments
;;; dependencies: print_string, print_hex
;;;


print_registers:
    pushf                       ; store flags before modification
    push bp
    mov bp, sp

    ; store register values not conserved by functions
    push dx                     ; dx in now in [bp-2]
    push cx                     ; cx in now in [bp-4]
    push ax                     ; ax in now in [bp-6]

    push .message
    call print_string

    ;----------
    ;--- AX ---
    ;----------
    mov ax, word [bp-6]         ; Restore ax. For reasons I don't understand,
                                ; mov sp, [bp-6] doesn't work
    push ax
    push .ax
    call .print_register


    ;----------
    ;--- BX ---
    ;----------
    push bx
    push .bx
    call .print_register

    ;----------
    ;--- CX ---
    ;----------
    mov cx, word [bp-4]         ; restore cx
    push cx
    push .cx
    call .print_register

    ;----------
    ;--- DX ---
    ;----------
    mov dx, word [bp-2]         ; restore dx
    push dx
    push .dx
    call .print_register

    ;----------
    ;--- SI ---
    ;----------
    push si
    push .si
    call .print_register

    ;----------
    ;--- DI ---
    ;----------
    push di
    push .di
    call .print_register

    ;----------
    ;--- SP ---
    ;----------
    mov ax, word [bp]
    add ax, 6                   ; Original sp (2 bytes offset for each of
                                ; old bp, flags & return address)
    push ax
    push .sp
    call .print_register

    ;----------
    ;--- BP ---
    ;----------
    mov ax, word [bp]           ; original value of bp
    push ax
    push .bp
    call .print_register

    ;----------
    ;--- SS ---
    ;----------
    push ss
    push .ss
    call .print_register

    ;----------
    ;--- CS ---
    ;----------
    push cs
    push .cs
    call .print_register

    ;----------
    ;--- DS ---
    ;----------
    push ds
    push .ds
    call .print_register

    ;----------
    ;--- ES ---
    ;----------
    push es
    push .es
    call .print_register

    ;----------
    ;--- FS ---
    ;----------
    push fs
    push .fs
    call .print_register

    ;----------
    ;--- GS ---
    ;----------
    push gs
    push .gs
    call .print_register

    ;-------------
    ;--- FLAGS ---
    ;-------------
    mov ax, word [bp+2]
    push ax
    push .flags
    call .print_register

    push .newline
    call print_string

    mov sp, bp                  ; restore stack pointer
    pop bp
    add sp, 2                   ; 'pop' flags from stack
    ret


.message db "Registers:", 0
.flags:  db 0x0a, 0x0d, "flags: ", 0
.ax:     db 0x0a, 0x0d, "ax: ", 0
.bx:     db 0x0a, 0x0d, "bx: ", 0
.cx:     db 0x0a, 0x0d, "cx: ", 0
.dx:     db 0x0a, 0x0d, "dx: ", 0
.si:     db 0x0a, 0x0d, "si: ", 0
.di:     db 0x0a, 0x0d, "di: ", 0
.sp:     db 0x0a, 0x0d, "sp: ", 0
.bp:     db 0x0a, 0x0d, "bp: ", 0
.ss:     db 0x0a, 0x0d, "ss: ", 0
.cs:     db 0x0a, 0x0d, "cs: ", 0
.ds:     db 0x0a, 0x0d, "ds: ", 0
.es:     db 0x0a, 0x0d, "es: ", 0
.fs:     db 0x0a, 0x0d, "fs: ", 0
.gs:     db 0x0a, 0x0d, "gs: ", 0
.newline db 0x0a, 0x0d, 0

.print_register:                ; args: register name, register content
    push bp
    mov bp, sp

    mov ax, word [bp+4]         ; register name
    push ax
    call print_string

    mov ax, word [bp+6]         ; register contents
    push ax
    call print_hex

    mov sp, bp
    pop bp

    ret
