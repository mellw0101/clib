#!/bin/bash

# This script is used to compile the assembly code to object file

nasm -f elf64 -o obj/struct.o src/__asm__/struct.s