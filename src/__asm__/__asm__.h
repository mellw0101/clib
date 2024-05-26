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


// Inline assembly function to add two integers
int __asm__add(int a, int b);


// Wrapper functions in C
int my_get_field1();
int my_get_field2();

#endif // ____ASM____H__