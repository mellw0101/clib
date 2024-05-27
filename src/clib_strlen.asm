section .text
    global clib_strlen

clib_strlen:
    xor rax, rax        ; Clear RAX (this will be the length)
    xor rcx, rcx        ; Clear RCX (this will be our counter)

.loop:
    mov al, [rdi + rcx] ; Load byte from string into AL
    test al, al         ; Test if AL is zero
    je .done            ; If zero, we are done
    inc rcx             ; Increment counter
    jmp .loop           ; Repeat

.done:
    mov rax, rcx        ; Move the counter to RAX (return value)
    ret