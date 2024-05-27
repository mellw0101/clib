section .data
    msg db 'Hello, world!', 0

section .text
    .globl _start

_start:
    ; Write the message to stdout (file descriptor 1)
    mov rax, 1          ; system call number for sys_write
    mov rdi, 1          ; file descriptor (stdout)
    mov rsi, msg        ; address of the message
    mov rdx, 13         ; length of the message
    syscall             ; make the system call

    ; Exit the program
    mov rax, 60         ; system call number for sys_exit
    xor rdi, rdi        ; exit code 0
    syscall             ; make the system call
