#include <stddef.h>

extern void* asm_memcpy(void* dest, const void* src, size_t n);

void* clib_memcpy(void* dest, const void* src, size_t n) {
    return asm_memcpy(dest, src, n);
}