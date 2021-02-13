;;;
;;; print a word as a hexadecimal number
;;; 1 argument: the hex value to print
;;; dependencies: print_string
;;;

print_hex:
    push bp
    mov bp, sp
    push di

    mov dx, word [bp+4]         ; move hex value to print to ax
    mov di, .hex_string + 2
    mov cx, 4

.loop:
    mov al, dh
    shr al, 4                   ; isolate four most significant bits

    cmp al, 0x09                ; handle 0-9 and a-f separately
    ja .gtea
    jmp .lte9

.lte9:
    add al, '0'
    jmp .add_to_string

.gtea:
    add al, 'a'-0x0a
    jmp .add_to_string

.add_to_string:
    stosb
    shl dx, 4                   ; handle 4 next most significant bits

    loop .loop

.end:
    push .hex_string
    call print_string
    add sp, 2                   ; 'pop' hex string address

    pop di
    pop bp

    ret

.hex_string: db "0x0000", 0
.test_string: db "testest", 0
