; section .text
;     global add_numbers

; add_numbers:
;     ; Arguments: rdi = a, rsi = b
;     add rdi, rsi
;     mov rax, rdi
;     ret

section .text
    global add_numbers

add_numbers:
    ; Function prologue
    push rbp
    mov rbp, rsp

    ; Move parameters into registers
    mov rax, rdi   ; rax = first parameter (x)
    add rax, rsi   ; rax += second parameter (y)

    ; Function epilogue
    mov rsp, rbp
    pop rbp
    ret