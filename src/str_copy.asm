section .data
str1: db "Hello, ", 0
str2: db "ARM Assembly!", 0

section .bss
buffer resb 256  ; Reserve 256 bytes for the buffer

section .text
global _start

_start:
    ; Copy str1 to buffer
    lea rdi, [buffer]  ; Load address of buffer into rdi
    lea rsi, [str1]    ; Load address of str1 into rsi
    call asm_strcpy    ; Copy str1 to buffer

    ; Append str2 to buffer
    lea rdi, [buffer]  ; Load address of buffer into rdi
    call asm_strlen    ; Get the length of the string in buffer
    add rdi, rax       ; Move rdi to the end of the string
    lea rsi, [str2]    ; Load address of str2 into rsi
    call asm_strcpy    ; Copy str2 to the end of buffer

    ; Exit the program (Linux syscall for exit)
    mov rax, 60        ; syscall number for exit
    xor rdi, rdi       ; exit code 0
    syscall            ; invoke syscall

asm_strlen:
    xor rcx, rcx                ; Clear RCX (this will be our counter)
.loop:
    cmp byte [rdi + rcx], 0     ; Compare byte at [rdi + rcx] with 0
    je .done                    ; If zero, we are done
    inc rcx                     ; Increment counter
    jmp .loop                   ; Repeat
.done:
    mov rax, rcx                ; Move the counter to RAX (return value)
    ret                         ; Return

asm_strcpy:
.loop:
    mov al, [rsi]               ; Load a byte from the source
    mov [rdi], al               ; Store the byte into the destination
    inc rsi                     ; Increment source pointer
    inc rdi                     ; Increment destination pointer
    test al, al                 ; Check if the byte was zero
    jnz .loop                   ; If not zero, continue copying
    ret                         ; Return