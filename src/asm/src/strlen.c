#include <stddef.h>

extern size_t asm_strlen(const char* str);

size_t clib_strlen(const char* str) {
    return asm_strlen(str);
}