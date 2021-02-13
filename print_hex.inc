;;;
;;; print a word as a hexadecimal number
;;; 1 argument: the hex value to print
;;; dependencies: print_string
;;;

print_hex:
    push bp
    mov bp, sp
    push bx

    mov ax, word [bp+4]         ; move hex value to print to ax
    mov bx, .hex_string + 2
    mov cx, 4

.loop:
    mov dh, ah
    shr dh, 4                   ; isolate four most significant bits

    cmp dh, 0x09                ; handle 0-9 and a-f separately
    ja .gtea
    jmp .lte9

.lte9:
    add dh, '0'
    jmp .add_to_string

.gtea:
    add dh, 'a'-0x0a
    jmp .add_to_string

.add_to_string:
    mov byte [bx], dh
    shl ax, 4                   ; handle 4 next most significant bits
    inc bx

    loop .loop

.end:
    push .hex_string
    call print_string
    add sp, 2                   ; 'pop' hex string address

    pop bx
    pop bp

    ret

.hex_string: db "0x0000", 0
.test_string: db "testest", 0
