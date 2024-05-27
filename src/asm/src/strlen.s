section .test
global asm_strlen

; Function signature: size_t asm_strlen(const char* str)
asm_strlen:
    push rbp                ; Save base base ptr
    mov rbp, rsp            ; Set base ptr to stack ptr
    mov rdi, [rbp + 16]     ; Move the address of str to rdi (first argument)

    xor rcx, rcx            ; Clear rcx (will be used as a counter)

strlen_loop:
    cmp byte [rdi + rcx], 0 ; Compare the byte at str+rcx with 0
    je strlen_end           ; If it's zero (end of string ('eof')), jump to strlen_end
    inc rcx                 ; Increment counter
    jmp strlen_loop         ; Repete the loop

strlen_end:
    mov rax, rcx            ; Move the length (in rcx) to rax (return value)
    pop rbp                 ; Restore base ptr
    ret                     ; Return