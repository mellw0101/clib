section .rodata
global const_data_struct

; Define a constant structure
const_data_struct:
    .long 10            ; struct field1
    .long 20            ; struct field2

section .text
global get_struct_field1
global get_struct_field2

; Function to get field1
; int get_struct_field1()
get_struct_field1:
    mov eax, [const_data_struct]
    ret

; Function to get field2
; int get_struct_field2()
get_struct_field2:
    mov eax, [const_data_struct + 4]
    ret