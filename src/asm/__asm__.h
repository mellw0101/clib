#pragma once

#ifndef ____ASM____H__
#define ____ASM____H__
    #ifndef __asm__call__function__
    #define __asm__call__function__
        #define __asm__call(__name, ...)                                                                                       \
            int result;                                                                                                        \
            __asm__ volatile("call " #__name "\n"                                                                              \
                            "movl %%eax, %0\n"                                                                                \
                            : "=r"(result)                                                                                    \
                            : "D"(__VA_ARGS__)                                                                                \
                            : "%eax");                                                                                        \
            if (result < 0)                                                                                                    \
            {                                                                                                                  \
                printf("__asm__call(" #__name ") failed with code %d\n", result);                                              \
                errno = -result;                                                                                               \
            }
    #endif // __asm__call__function__

#ifdef size_t
    #undef size_t
    #define size_t unsigned long int
#elif !defined(size_t)
    #define size_t unsigned long int
#endif // size_t
static inline void* inline_asm_memcpy(void* dest, const void* src, size_t n)
{
    // Cast input pointers to char* for byte-wise copying
    char*       d = (char*)dest;
    const char* s = (const char*)src;

    asm volatile(
        "rep movsb"                 // Repeat move string byte
        : "+D"(d), "+S"(s), "+c"(n) // Output and input operands
        :                           // No additional input-only operands
        : "memory"                  // Clobbered registers
    );
    return dest;
}
#undef size_t

#include "src/asm/include/io.h"
#include "src/asm/include/strlen.h"

#endif // ____ASM____H__