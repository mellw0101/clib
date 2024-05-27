#include <stdint.h>

// Declaration of the assembly function
extern uint64_t asm_read_register();

// Wrapper function in C
uint64_t clib_read_register() {
    return asm_read_register();
}