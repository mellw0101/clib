section .text
    global get_status_and_value
; Function signature: unsigned long get_status_and_value(unsigned long input)
; Returns: [status (upper 32 bits) | value (lower 32 bits)]
get_status_and_value:
    push rbp
    mov rbp, rsp

    mov rax, [rbp + 16]   ; Load input into rax
    mov ecx, 1            ; Status code (example: 1 for success)
    mov edx, eax          ; Value to return (copy of input)
    shl rcx, 32           ; Shift status code to the upper 32 bits
    or rax, rcx           ; Combine status code and value into rax

    pop rbp
    ret