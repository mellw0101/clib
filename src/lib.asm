section .data
    NULL equ 0
    SUCCESS equ 0
    FAILURE equ 1
    true equ 1
    false equ 0
    newline db 0xA, NULL
    int_asm_print_int_buf db 20 dup(0)
    str_asset_failed db "Assertion failed", 0xA, NULL

section .bss
    ; Buffer for reading input
    input_buffer resb 256

section .text
    global asm_print_str
    global asm_exit
    global asm_strlen
    global asm_exit_SUCCESS
    global asm_exit_FAILURE
    global asm_print_int
    global asm_add
    global asm_sub
    global asm_mul
    global asm_div
    global asm_print
    global asm_strcpy
    global asm_assert
    global asm_assert_str_msg
    global asm_malloc
    global asm_append_str
    global asm_errno
    global asm_execve
    global asm_find
    global asm_int_to_str

    ; External declaration of the brk system call
    extern brk
    ; Define the asm_malloc function
    ; Declare the function as global (externally visible)

%define qMAX -1

%macro __FUNC_START__ 0
    push rbp
    mov rbp, rsp
%endmacro

%macro __FUNC_END__ 0
    mov rsp, rbp
    pop rbp
    ret
%endmacro

%macro __RETURN 1
    mov rax, %1
    ret
%endmacro

%macro __ASM_PRINT 1
    cmp %1, rdi
    je .print
    mov rdi, %1
    .print:
        call asm_print_str
%endmacro

; section .data
;     __exit_failure_msg db "Exiting program: FAILURE", newline
;     __exit_success_msg db "Exiting program: SUCCESS", newline

; %macro __ASM_EXIT
;     cmp rax, EXIT_SUCCESS       ; Check if the return value is EXIT_SUCCESS
;     je .exit_success            ; If equal, jump to exit_success
;     jmp .exit_failure           ; Otherwise, jump to exit_failure
;     .exit_success:
;         __ASM_PRINT __exit_message
;         mov rax, EXIT_SUCCESS
;         jmp .exit
;     .exit_failure:
;         __ASM_PRINT __exit_message
;         mov rax, EXIT_FAILURE
;     .exit:
;         call asm_exit
; %endmacro

%macro __RET 1
    mov rax, %1
    ret
%endmacro

%define EXIT_SUCCESS 0
%define EXIT_FAILURE 1

__return_address:
    mov rax, [rbp + 8]
    ret

__return_address_2:
    mov rax, [rbp + 16]
    ret

__return_NULL:
    mov rax, NULL
    ret

__return_SUCCESS:
    mov rax, EXIT_SUCCESS
    ret

__return_FAILURE:
    mov rax, EXIT_FAILURE
    ret



asm_exit:
    ;
    ;  asm_exit(status) -> void
    ;
    mov rax, 60     ; System call number for sys_exit
    syscall         ; Make the system call

asm_exit_SUCCESS:
    mov rdi, EXIT_SUCCESS
    call asm_exit
    __RET EXIT_SUCCESS

asm_exit_FAILURE:
    mov rdi, EXIT_FAILURE
    call asm_exit
    jmp __return_FAILURE

asm_print_str:
    ;  
    ;   Description: Prints a null-terminated string to stdout
    ;   Signature: asm_print_str(const char *str) -> size_t
    ;   Arguments: rdi = str
    ;   Returns: 0 on success, -1 on failure
    ;
    __FUNC_START__
    mov rsi, rdi                        ; Move the string pointer to rsi (second argument for write)
    mov rdx, 0                          ; Initialize rdx to 0 for length calculation

    .find_len:
        cmp byte [rsi + rdx], NULL      ; Compare each byte to 0 (null terminator)
        je .write_string                ; If null terminator, jump to write_string
        inc rdx                         ; Otherwise, increment rdx
        jmp .find_len                   ; Repeat until null terminator

    .write_string:
        mov rax, 1                      ; System call number for sys_write
        mov rdi, 1                      ; File descriptor 1 (stdout)
        syscall                         ; Make the system call
    mov rax, 0                          ; Set the return value to 0
    __FUNC_END__

__asm_print_nl:
    mov rdi, newline
    call asm_print_str
    jmp __return_SUCCESS

asm_strlen:
    xor rcx, rcx                ; Clear RCX (this will be our counter)

.loop:
    cmp byte [rdi + rcx], NULL  ; Load byte from string into AL
    je .done                    ; If zero, we are done
    inc rcx                     ; Increment counter
    jmp .loop                   ; Repeat

.done:
    xor rax, rax                ; Clear RAX
    mov rax, rcx                ; Move the counter to RAX (return value)
    ret                         ; Return

asm_print_int:
    __FUNC_START__
    ; Body
    mov rax, rdi
    mov rbx, int_asm_print_int_buf + 19
    mov byte [rbx], NULL

    .convert_loop:          ; Convert integer to string
        mov rcx, 10         ; Divisor
        xor rdx, rdx        ; Clear rdx
        div rcx             ; Divide rax by 10
        add dl, '0'         ; Convert remainder to ASCII
        dec rbx             ; Move buffer pointer
        mov [rbx], dl       ; Store ASCII character
        test rax, rax       ; Check if quotient is zero
        jnz .convert_loop   ; If not, continue

    mov rsi, rbx                            ; Move the string pointer to rsi (second argument for write)
    mov rdx, int_asm_print_int_buf + 19     ; Calculate length of string
    sub rdx, rbx                            ; Subtract buffer pointer from end of buffer
    mov rax, 1                              ; System call number for sys_write
    mov rdi, 1                              ; File descriptor 1 (stdout)
    syscall                                 ; Make the system call
    __FUNC_END__

asm_add:            ; Function signature: add_numbers(a, b)
    ; Prologue
    push rbp        ; Set up stack frame
    mov rbp, rsp    ; Save base pointer

    ; Body
    mov rax, rdi    ; Move first param (rdi) to rax
    add rax, rsi    ; Add second param (rsi) to rax

    ; Epilogue
    mov rsp, rbp    ; Restore stack pointer
    pop rbp         ; Restore base pointer
    ret             ; Returns: register rax

asm_sub:
    ; Prologue
    push rbp        ; Set up stack frame
    mov rbp, rsp    ; Save base pointer

    ; Body
    mov rax, rdi    ; Move first param (rdi) to rax
    sub rax, rsi    ; Subtract second param (rsi) from rax

    ; Epilogue
    mov rsp, rbp    ; Restore stack pointer
    pop rbp         ; Restore base pointer
    ret             ; Returns: register rax
asm_mul:
    ; Prologue
    push rbp        ; Set up stack frame
    mov rbp, rsp    ; Save base pointer

    ; Body
    mov rax, rdi    ; Move first param (rdi) to rax
    imul rax, rsi   ; Multiply rax by rsi

    ; Epilogue
    mov rsp, rbp    ; Restore stack pointer
    pop rbp         ; Restore base pointer
    ret             ; Returns: register rax

asm_div:
    ;
    ; asm_div(a, b) -> long
    ;
    __FUNC_START__

    cmp rsi, 0          ; Check if divisor is zero
    je .div_by_zero     ; If zero, jump to div_by_zero
    mov rax, rdi        ; Move first param (rdi) to rax
    xor rdx, rdx        ; Clear rdx
    div rsi             ; Divide rax by rsi
    jmp .end            ; Jump to end

    .div_by_zero:
        mov rax, 0      ; Set return value to 0
    .end:
        __FUNC_END__

asm_strcpy:
    ; Prologue
    push rbp
    mov rbp, rsp

    ; Function arguments
    ; rdi: destination string (dest)
    ; rsi: source string (src)

    mov rdx, rdi  ; Save the original destination pointer to return it later

.copy_loop:
    mov al, [rsi] ; Load the byte at the source pointer into al
    mov [rdi], al ; Store the byte from al to the destination pointer
    inc rsi       ; Move to the next byte in the source string
    inc rdi       ; Move to the next byte in the destination string
    test al, al   ; Test if the byte is zero (null terminator)
    jnz .copy_loop ; If not zero, continue copying

    mov rax, rdx  ; Return the original destination pointer

    ; Epilogue
    mov rsp, rbp
    pop rbp
    ret
    
asm_assert:                     ; Function signature: assert_equal(a, b) -> void
    cmp rdi, rsi                ; Compare the two arguments
    jne .failed                 ; If not equal, jump to assert_failed
    jmp .passed                 ; Otherwise, jump to assert_passed

.failed:
    mov rdi, str_asset_failed   ; Load the address of the error message
    call asm_print_str          ; Print the error message
    jmp asm_exit_FAILURE        ; Exit with failure

.passed:
    jmp __return_SUCCESS        ; Return success

asm_assert_str_msg:             ; Function signature: assert_equal_str_msg(a, b, msg) -> void
    cmp rdi, rsi                ; Compare the two arguments
    jne .failed                 ; If not equal, jump to assert_failed
    jmp .passed                 ; Otherwise, jump to assert_passed

.failed:
    mov rdi, rdx                ; Load the address of the error message
    call asm_print_str          ; Print the error message
    jmp asm_exit_FAILURE        ; Exit with failure

.passed:
    __RETURN EXIT_SUCCESS

asm_malloc:
    ;
    ; Description: Allocate memory using the brk system call
    ; Function arguments
    ; rdi: size of memory to allocate
    ;
    __FUNC_START__
    ; Save the requested size
    mov rsi, rdi

    ; Call sbrk(0) to get the current program break
    mov rax, 12         ; syscall number for brk
    xor rdi, rdi        ; NULL pointer to get current break
    syscall

    ; rax now contains the current program break (old_brk)
    mov rbx, rax        ; save the current break in rbx

    ; Calculate the new program break
    add rbx, rsi        ; new_brk = old_brk + size

    ; Call brk(new_brk) to set the new program break
    mov rax, 12         ; syscall number for brk
    mov rdi, rbx        ; new program break
    syscall

    ; Check if brk failed
    cmp rax, rbx
    jne .alloc_fail     ; If not equal, allocation failed

    ; Allocation successful, return the old program break
    mov rax, rbx
    sub rax, rsi
    jmp .exit
    
.alloc_fail: ; Allocation failed, return NULL (0)
    xor rax, rax

.exit:
    __FUNC_END__

asm_append_str:
    ; append_str(dest, src) -> void
    ;
    ; Description: Appends a null-terminated string to another null-terminated string
    ; Arguments: rdi: destination string (dest)
    ; rdi: destination string (dest)
    ; rsi: source string (src)
    ;

    ; Prologue
    push rbp                    ; Save the base pointer
    mov rbp, rsp                ; Set the base pointer to the current stack pointer
    mov rcx, rdi                ; Save the destination pointer to rcx

    .find_null:                 ; Find the null terminator of the destination string
        cmp byte [rcx], NULL    ; Compare the byte at the destination pointer to 0 (null terminator)
        je .copy_loop           ; If null terminator, jump to copy_loop
        inc rcx                 ; Move to the next byte in the destination string
        jmp .find_null          ; Repeat

    .copy_loop:
        mov al, [rsi]           ; Load the byte at the source pointer into al
        mov [rcx], al           ; Store the byte from al to the destination pointer
        inc rsi                 ; Move to the next byte in the source string
        inc rcx                 ; Move to the next byte in the destination string
        test al, al             ; Test if the byte is zero (null terminator)
        jnz .copy_loop          ; If not zero, continue copying

    ; Epilogue
    mov rsp, rbp                ; Restore the stack pointer
    pop rbp                     ; Restore the base pointer
    mov rax, rax                ; set rax reg to NULL
    ret                         ; Return rax reg

asm_errno: ; Function signature: asm_errno(str) -> int
    ;
    ; Description: Returns the value of the errno variable
    ;
    ; Arguments:
    ; rdi: error message string
    ;
    ; Returns:
    ; rax: errno value
    ;
    __FUNC_START__
    
    mov rax, 0                  ; Clear rax register
    mov rax, [fs:0x28]          ; Get the errno value
    __ASM_PRINT rdi             ; Print the error message
    mov rdx, 0                  ; Initialize rdx to 0 for length calculation
    ; Get the errno value
    mov rax, 0x4                ; mov 0x4 to reg rax ( return reg ) syscall number for sys_write
    mov rdi, 0x2                ; file descriptor 2 (stderr)
    syscall                     ; make the system call
    cmp rax, 0                  ; Check if the system call failed
    jne .error                  ; If not equal, jump to error
    mov rax, EXIT_SUCCESS       ; Set the return value to 0
    jmp .end                    ; Otherwise, jump to end
    .error:
        mov rax, EXIT_FAILURE       ; Clear rax register
    .end:
        __FUNC_END__

asm_execve:
    ;
    ; Description: Execute a program
    ; Function arguments
    ; rdi: program name
    ; rsi: arguments
    ; Returns: 0 on success, 1 on failure
    ;
    __FUNC_START__
    cmp byte [rdi], NULL        ; Check if the program name is NULL
    je .failure                 ; If NULL, exit the program
    cmp byte [rsi], NULL        ; Check if the arguments are NULL
    je .failure                 ; If NULL, exit the program
    mov rdx, rsi                ; Move the arguments to rdx
    mov rax, 59                 ; syscall number for execve
    syscall                     ; Make the system call
    mov rax, EXIT_SUCCESS       ; Set the return value to 0
    jmp .exit                   ; Jump to exit
    .failure:                   ; If the program name or arguments are NULL
        mov rax, EXIT_FAILURE   ; Set the return value to 1
    .exit:                      ; Exit the function
        __FUNC_END__            ; End the function


asm_find:
    ;
    ; Description: Find a character in a string
    ; Function arguments
    ; rdi: string
    ; rsi: character to find
    ; Returns: pointer to the first occurrence of the character, NULL if not found
    ;
    __FUNC_START__
    mov rcx, rdi                ; Move the string pointer to rcx
    mov al, rsi                 ; Move the character to find to al
    .find_loop:                 ; Find the character in the string
        cmp byte [rcx], NULL    ; Check if the end of the string is reached
        je .not_found           ; If NULL, jump to not_found
        cmp byte [rcx], al      ; Compare the character in the string to the character to find
        je .found               ; If equal, jump to found
        inc rcx                 ; Move to the next character in the string
        jmp .find_loop          ; Repeat

    .found:                     ; Character found
        mov rax, rcx            ; Move the pointer to the character to rax
        jmp .exit               ; Jump to exit

    .not_found:                 ; Character not found
        mov rax, NULL           ; Set the return value to NULL

    .exit:                      ; Exit the function
        __FUNC_END__            ; End the function