section .text
global asm_memcpy

; rbp = base ptr
; rdi = dest = (rbp + 16) = (base ptr + ptr)
; rsi = src = (rbp + 24) = (base ptr + ptr + ptr)
;
; Function signature: void* asm_memcpy(void* dest, const void* src, size_t n)
asm_memcpy:
    push rbp            ; Save base ptr
    mov rbp, rsp        ; Move base ptr to stack ptr
    mov rdi, [rbp + 16] ; Move the address of dest (first param arg) to rdi 
    mov rsi, [rbp + 24] ; Move the Address of src (second param arg) to rsi
    mov rcx, [rbp + 32] ; Null terminate

memcpy_loop:
    cmp rcx, 0          ; Compare value of rcx to zero
    je memcpy_end       ; If rcx is zero jump to memcpy_end
    mov al, [rsi]       ; Move byte of src (mem address) to al
    mov [rdi], al       ; Move byte al to address of dest
    inc rsi             ; Incrament src ptr (rsi + byte('+8'))
    inc rdi             ; Incrament dest ptr (rdi + byte('+8'))
    dec rcx             ; Decrement rcx ptr
    jmp memcpy_loop     ; Repete loop

memcpy_end:
    mov rax, [rbp + 16] ; Move dest ptr (rbp('base ptr') + 16) to return dest
    pop rbp             ; Restore base ptr
    ret                 ; Return