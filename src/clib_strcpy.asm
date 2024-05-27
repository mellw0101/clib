section .data
    NULL_FAILURE equ 0
    FAILURE equ 0
    SUCCESS equ 1
    EINVAL equ 22
    ENOMEM equ 12

section .text
    global clib_strcpy

clib_strcpy:
    push rbp                 ; Save base pointer
    mov rbp, rsp             ; Set base pointer to stack pointer
    sub rsp, 32              ; Allocate space on stack (optional, for alignment purposes)
    
    ; Save registers that will be used
    push rbx
    push rsi
    push rdi

    ; Get arguments from stack
    mov rdi, [rbp + 16]      ; dest
    mov rsi, [rbp + 24]      ; src
    mov rdx, [rbp + 32]      ; dest_size

    ; Check if either pointer is null
    test rdi, rdi
    jz .null_failure
    test rsi, rsi
    jz .null_failure

    ; Calculate the length of dest
    xor rcx, rcx             ; Clear rcx (dest_len)
    mov rbx, rdi             ; Save original dest pointer

.calculate_dest_len:
    cmp byte [rdi + rcx], 0 ; Compare the byte at dest + rcx with null character
    je .dest_len_done        ; If it's null, we're done
    inc rcx                  ; Increment the length
    jmp .calculate_dest_len  ; Repeat the loop

.dest_len_done:
    mov r8, rcx              ; Save dest_len

    ; Calculate the length of src
    xor rcx, rcx             ; Clear rcx (src_len)
    mov rdi, rsi             ; Use rdi for src pointer

.calculate_src_len:
    cmp byte [rdi + rcx], 0 ; Compare the byte at src + rcx with null character
    je .src_len_done         ; If it's null, we're done
    inc rcx                  ; Increment the length
    jmp .calculate_src_len   ; Repeat the loop

.src_len_done:
    mov r9, rcx              ; Save src_len

    ; Check if there is enough space in dest buffer
    mov rax, r8              ; dest_len
    add rax, r9              ; dest_len + src_len
    add rax, 1               ; dest_len + src_len + 1
    cmp rax, rdx             ; Compare with dest_size
    ja .mem_failure          ; If not enough space, jump to memory failure

    ; Find the end of the destination string again
    mov rdi, rbx             ; Restore original dest pointer

.find_end_of_dest:
    cmp byte [rdi], 0       ; Compare the byte at dest with null character
    je .end_found            ; If it's null, end found
    inc rdi                  ; Increment dest pointer
    jmp .find_end_of_dest    ; Repeat the loop

.end_found:
    ; Append the source string to the destination string
    mov rsi, [rbp + 24]      ; Restore src pointer

.append_loop:
    mov al, [rsi]            ; Load the byte from src into al
    cmp al, 0                ; Compare it with null character
    je .append_done          ; If it's null, we're done
    mov [rdi], al            ; Store the byte in dest
    inc rdi                  ; Move to the next byte of dest
    inc rsi                  ; Move to the next byte of src
    jmp .append_loop         ; Repeat the loop

.append_done:
    mov byte [rdi], 0        ; Null-terminate the resulting string
    mov eax, SUCCESS         ; Set return value to 1 (success)
    jmp .cleanup             ; Jump to cleanup

.null_failure:
    mov eax, EINVAL          ; Set errno to EINVAL
    mov eax, NULL_FAILURE    ; Set return value to 0 (null failure)
    jmp .cleanup             ; Jump to cleanup

.mem_failure:
    mov eax, ENOMEM          ; Set errno to ENOMEM
    mov eax, FAILURE         ; Set return value to 0 (memory failure)
    jmp .cleanup             ; Jump to cleanup

.cleanup:
    ; Restore registers
    pop rdi
    pop rsi
    pop rbx
    ; Restore base pointer and return
    mov rsp, rbp
    pop rbp
    ret