section .data
    EXIT_SUCCESS equ 0
    EXIT_FAILUER equ 1

section .text
    .globl _exit           ; make _exit visible to the linker
    .type _exit, @function ; declare _exit as a function
    ; _exit(int status)
_exit:
    cmp rdi, EXIT_SUCCESS  ; compare status with EXIT_SUCCESS
    mov eax, 1             ; syscall number for sys_exit
    mov ebx, eax           ; status
    int 0x80               ; call kernel
