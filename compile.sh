#!/bin/bash

# Ensure the script exits if any command fails
set -e

# Paths to the LLVM IR files and the C test program
EXIT_LL="clib_exit.ll"
CLOSE_LL="clib_close.ll"
PERROR_LL="clib_perror.ll"
OPEN_LL="clib_open.ll"
C_TEST_PROGRAM="clib.c"

# Check if the LLVM IR files and C test program exist
if [[ ! -f "$EXIT_LL" ]]; then
    echo "File $EXIT_LL not found!"
    exit 1
fi

if [[ ! -f "$CLOSE_LL" ]]; then
    echo "File $CLOSE_LL not found!"
    exit 1
fi

if [[ ! -f "$PERROR_LL" ]]; then
    echo "File $PERROR_LL not found!"
    exit 1
fi

if [[ ! -f "$OPEN_LL" ]]; then
    echo "File $OPEN_LL not found!"
    exit 1
fi

if [[ ! -f "$C_TEST_PROGRAM" ]]; then
    echo "File $C_TEST_PROGRAM not found!"
    exit 1
fi

# Compile the LLVM IR files to object files
clang -c -o clib_exit.o "$EXIT_LL"
clang -c -o clib_close.o "$CLOSE_LL"
clang -c -o clib_perror.o "$PERROR_LL"
clang -c -o clib_open.o "$OPEN_LL"

# Compile and link the C test program with the object files
gcc -o test_program "$C_TEST_PROGRAM" clib_exit.o clib_close.o clib_perror.o clib_open.o -lc

echo "Compilation successful. You can now run ./test_program"