// u8 append_str(char* dest, const char* src)
// {
//     long result;
//     asm volatile("movq %1, %%rdi\n" // Move the destination string to the first argument register (rdi)
//                  "movq %2, %%rsi\n" // Move the source string to the second argument register (rsi)
//                  "movl $1, %%edx\n" // Move the length of the source string to the third argument register (edx)
//                  "movl $9, %%eax\n" // Move the syscall number for sys_write to eax
//                  "syscall\n"        // Invoke the syscall
//                  "movq %%rax, %0\n" // Move the syscall result to the result variable
//                  : "=r"(result)
//                  : "r"(dest), "r"(src)
//                  : "%rdi", "%rsi", "%edx", "%eax");

//     // Set errno and handle errors if the system call failed
//     if (result < 0)
//     {
//         errno = -result;
//         perror("append_str_asm");
//         return FAILURE;
//     }

//     return SUCCESS;
// }

// u8 append_str(char* dest, const char* src)
// {
//     // Finding the end of the destination string
//     asm volatile("1: "
//                  "cmpb $0, (%0)\n" // Compare the byte at the current dest pointer with null character
//                  "je 2f\n"         // If it's null, jump to label 2
//                  "inc %0\n"        // Move to the next byte of dest
//                  "jmp 1b\n"        // Repeat the loop
//                  "2:\n"
//                  : "+r"(dest) // Output and input: updating the dest pointer
//     );

//     // Appending the source string to the destination string
//     asm volatile("3: "
//                  "movb (%1), %%al\n" // Load the byte from src into al
//                  "cmpb $0, %%al\n"   // Compare it with null character
//                  "je 4f\n"           // If it's null, jump to label 4
//                  "movb %%al, (%0)\n" // Store the byte in dest
//                  "inc %0\n"          // Move to the next byte of dest
//                  "inc %1\n"          // Move to the next byte of src
//                  "jmp 3b\n"          // Repeat the loop
//                  "4: "
//                  "movb $0, (%0)\n"       // Null-terminate the resulting string
//                  : "+r"(dest), "+r"(src) // Output and input: updating dest and src pointers
//                  :
//                  : "%al" // Clobber list: using al register
//     );

//     return SUCCESS;
// }
