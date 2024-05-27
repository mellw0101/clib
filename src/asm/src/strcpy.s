section .text
global asm_strcpy

; Function signature: char* asm_strspy(char* dest, const char* src)
asm_strcpy:
    push rbp                ; Save function base ptr
    mov rbp, rsp            ; Set function base ptr to stack ptr 
    mov rdi, [rbp + 16]     ; Move dest address to rdi (first argument)
    mov rsi, [rbp + 24]     ; Move src address to rsi (second argument)

strcpy_loop:
    mov al, [rsi]           ; Move the byte at src to al
    mov [rdi], al           ; Move the byte from al to dest
    inc rsi                 ; Increment src ptr
    inc rdi                 ; Incrament dets ptr
    test al, al             ; test if al is zero (end of string)
    jnz strcpy_loop         ; If not zero, repeat the strcpy_loop

    mov rax, [rbp + 16]     ; Move dest address to rax (return value)
    pop rbp                 ; Restore function base ptr
    ret                     ; Return