#!/bin/bash

comp_asm() {
    local file_name="$1"
    local asm_obj="obj/${file_name%.*}.o"
    local asm_src="src/${file_name%.*}.s"

    local c_obj="obj/${file_name%.*}_c.o"
    local c_src="src/${file_name%.*}.c"

    nasm -f elf64 -o "$asm_obj" "$asm_src"
    gcc -c -o "$c_obj" "$c_src"
}

comp_asm "io"
comp_asm "strlen"
comp_asm "strcpy"
comp_asm "memcpy"