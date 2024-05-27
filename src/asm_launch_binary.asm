section .data
    ; You can define any necessary constants or data here

section .text
    global asm_launch_binary       ; Make the function available globally

asm_launch_binary:
    ; Function signature: void asm_launch_binary(const char* const path, char* const* argv, char* const* envp)
    
    ; Save base pointer and set up the stack frame
    push rbp
    mov rbp, rsp
    
    ; Move function arguments into the appropriate registers for the execve syscall
    mov rdi, [rbp + 16]        ; path (first argument)
    mov rsi, [rbp + 24]        ; argv (second argument)
    mov rdx, [rbp + 32]        ; envp (third argument)
    
    ; Syscall number for execve is 59
    mov rax, 59
    
    ; Make the syscall
    syscall
    
    ; If execve returns, there was an error, so we should exit
    ; Exit syscall number is 60, exit code is 1 (indicating error)
    mov rax, 60
    mov rdi, 1
    syscall
    
    ; Mark the end of the function
    ; Restore base pointer and return
    mov rsp, rbp
    pop rbp
    ret

    section .text
    global _add_two_numbers ; Make the function globally accessible

_add_two_numbers:
    ; Prologue: Save the caller's stack frame pointer and set up a new one
    push ebp
    mov ebp, esp

    ; Function body: Perform the addition
    mov eax, [ebp + 8] ; Get the first argument (a)
    add eax, [ebp + 12] ; Add the second argument (b)

    ; Epilogue: Restore the caller's stack frame pointer and return
    mov esp, ebp
    pop ebp
    ret