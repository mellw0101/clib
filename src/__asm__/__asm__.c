#include "__asm__.h"
/**
    @file clib.c
    @brief Contains the definition of the __asm__call macro.

    The __asm__call macro is used to invoke assembly language functions from C code.
    It takes the name of the assembly function as a parameter, along with any additional
    arguments required by the function. The macro uses inline assembly to pass the arguments
    and call the assembly function, and then stores the result in the 'result' variable.
    If the result is less than 0, it prints an error message and sets the 'errno' variable
    to the negative value of the result.
*/
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

int __asm__add(int const a, int const b)
{
    int result;
    __asm__ 
    (
        "addl %%ebx, %%eax;"
        : "=a" (result)
        : "a" (a), "b" (b)
    );

    return result;
}

// Declarations of the assembly functions
extern int get_field1();
extern int get_field2();

// Wrapper functions in C
int my_get_field1()
{
    return get_field1();
}

int my_get_field2()
{
    return get_field2();
}