section .text
global asm_read_register

; Function to read a value from a hardware register (example address 0x40000000)
asm_read_register:
    mov rax, 0x40000000  ; Address of the hardware register
    mov rax, [rax]       ; Read the value from the register
    ret