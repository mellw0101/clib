 section .data
     NULL equ 0
     SUCCESS equ 0
     FAILURE equ 1
     EXIT_SUCCESS equ 0
     EXIT_FAILURE equ 1
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

     ; External declaration of the brk system call
     extern brk
     ; Define the asm_malloc function
     ; Declare the function as global (externally visible)

 __return_address:
     mov rax, [rbp + 8]
     ret

 __return_address_2:
     mov rax, [rbp + 16]
     ret

 __return_NULL:
     xor rax, rax
     ret

 __return_SUCCESS:
     mov rax, EXIT_SUCCESS
     ret

 __return_FAILURE:
     mov rax, EXIT_FAILURE
     ret

 asm_exit:
     ;
     ;   Description: Exits the program with the given exit code
     ;   Signature: asm_exit(long code) -> void
     ;   Arguments: rdi = code
     ;   Returns: None (exits the program) __builtin_return_address(0)
     ;
 .Prologue:
     push rbp        ; Save the base pointer (rbp)
     mov rbp, rsp    ; Set the base pointer to the current stack pointer (rsp)

 .Body:
     mov rax, 60     ; System call number for sys_exit
     syscall         ; Make the system call

 .end:
     mov rsp, rbp    ; Restore the stack pointer
     pop rbp         ; Restore the base pointer

 asm_exit_SUCCESS:
     mov rdi, EXIT_SUCCESS
     call asm_exit
     jmp __return_SUCCESS

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

     ; Prologue
     push rbp                            ; Save the base pointer
     mov rbp, rsp                        ; Set the base pointer to the current stack pointer

     ; Body
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

     ; Epilogue
     mov rsp, rbp                        ; Restore the stack pointer
     pop rbp                             ; Restore the base pointer
     jmp __return_SUCCESS                ; Return from function

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
     ; Prologue
     push rbp
     mov rbp, rsp

     ; Body
     mov rax, rdi
     mov rbx, int_asm_print_int_buf + 19
     mov byte [rbx], NULL

 convert_loop:           ; Convert integer to string
     mov rcx, 10         ; Divisor
     xor rdx, rdx        ; Clear rdx
     div rcx             ; Divide rax by 10
     add dl, '0'         ; Convert remainder to ASCII
     dec rbx             ; Move buffer pointer
     mov [rbx], dl       ; Store ASCII character
     test rax, rax       ; Check if quotient is zero
     jnz convert_loop    ; If not, continue

     ; Body
     mov rsi, rbx                            ; Move the string pointer to rsi (second argument for write)
     mov rdx, int_asm_print_int_buf + 19     ; Calculate length of string
     sub rdx, rbx                            ; Subtract buffer pointer from end of buffer
     mov rax, 1                              ; System call number for sys_write
     mov rdi, 1                              ; File descriptor 1 (stdout)
     syscall                                 ; Make the system call

     ; Epilogue
     mov rsp, rbp                            ; Restore the stack pointer
     pop rbp                                 ; Restore the base pointer
     ret                                     ; Return

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
     ; Prologue
     push rbp                ; Set up stack frame
     mov rbp, rsp            ; Save base pointer

     ; Body
     cmp rsi, 0              ; Check if divisor is zero
     je .div_by_zero         ; If zero, jump to div_by_zero
     mov rax, rdi            ; Move first param (rdi) to rax
     xor rdx, rdx            ; Clear rdx
     div rsi                 ; Divide rax by rsi

     ; Epilogue
     mov rsp, rbp            ; Restore stack pointer
     pop rbp                 ; Restore base pointer
     ret                     ; Returns: register rax
 .div_by_zero:
     ; Epilogue
     mov rsp, rbp            ; Restore stack pointer
     pop rbp                 ; Restore base pointer
     jmp __return_FAILURE    ; Return failure

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
     jmp __return_SUCCESS        ; Return success


 asm_malloc:
     ;
     ; Description: Allocate memory using the brk system call
     ; Function arguments
     ; rdi: size of memory to allocate
     ;

     ; Prologue
     push rbp
     mov rbp, rsp

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

     ; Epilogue
     mov rsp, rbp
     pop rbp
     ret

 .alloc_fail:
     ; Allocation failed, return NULL (0)
     xor rax, rax

     ; Epilogue
     mov rsp, rbp
     pop rbp
     ret


 asm_append_str:                 ; Function signature: asm_append_str(dest, src) -> void
     ; Function arguments
     ; rdi: destination string (dest)
     ; rsi: source string (src)

     ; Prologue
     push rbp                    ; Save the base pointer
     mov rbp, rsp                ; Set the base pointer to the current stack pointer

     ; Find the null terminator of the destination string
     mov rcx, rdi
     .find_null:
         cmp byte [rcx], NULL
         je .copy_loop
         inc rcx
         jmp .find_null

     .copy_loop:
         mov al, [rsi] ; Load the byte at the source pointer into al
         mov [rcx], al ; Store the byte from al to the destination pointer
         inc rsi       ; Move to the next byte in the source string
         inc rcx       ; Move to the next byte in the destination string
         test al, al   ; Test if the byte is zero (null terminator)
         jnz .copy_loop ; If not zero, continue copying

     ; Epilogue
     mov rsp, rbp
     pop rbp
     ret