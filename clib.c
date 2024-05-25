#include "clib.h"

#include "errno.h"
#include "pwd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/stat.h"
#include "unistd.h"

void read_from_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // Process the read data
        printf("%s", buffer);
    }

    fclose(file);
}


void write_to_file(const char* filename, const char* data)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fputs(data, file);

    fclose(file);
}


size_t slen(const char* s)
{
    size_t i = 0;
    for (; s[i]; ++i)
        ;
    return i;
}


void strcp(char* dest, const char* src)
{
    size_t i = 0;
    for (; src[i]; ++i)
        dest[i] = src[i];
    dest[i] = '\0';
}


UserInfo* get_user_info(void)
{
    UserInfo* info = malloc(sizeof(UserInfo));
    if (info == NULL)
    {
        // Error handling if malloc fails
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memset(info, 0, sizeof(UserInfo));

    // Get the current user
    const char* username = getlogin();
    if (username != NULL)
    {
        strncpy(info->user, username, slen(username) + 1);
    }
    // Get the current working directory
    if (getcwd(info->cwd, sizeof(info->cwd)) == NULL)
    {
        // Error handling if getcwd fails
        perror("getcwd");
        free(info);
        exit(EXIT_FAILURE);
    }

    return info;
}

// Custom attributes for GCC
#ifndef __THROW
    #define __THROW
#endif

#ifndef __nonnull
    #define __nonnull(params) __attribute__((nonnull params))
#endif

#ifndef __wur
    #define __wur __attribute__((warn_unused_result))
#endif

// Custom function declaration with attributes
extern int my_chdir(const char* path) __THROW __nonnull((1)) __wur;

// Custom function implementation
int my_chdir(const char* path)
{
    // Check if the path is non-null (although __nonnull should ensure this)
    if (!path)
    {
        fprintf(stderr, "Path cannot be null.\n");
        return -1;
    }

    // Change directory and return the result
    int result = chdir(path);
    if (result != 0)
    {
        perror("chdir");
        return -1;
    }

    return 0;
}


void change_directory(const char* const path, const char* const user)
{
    if (!user)
    {
        printf("User not specified\n");
        exit(EXIT_FAILURE);
    }

    if (chdir(path) != 0)
    {
        // Prompt for sudo password and execute chown command
        printf("Enter sudo password: ");
        char password[256];
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0'; // Remove trailing newline character

        char command[256];
        snprintf(command, sizeof(command), "echo %s | sudo -S chown -R %s:%s %s", password, user, "wheel", path);
        if (system(command) != 0)
        {
            perror("system");
            exit(EXIT_FAILURE);
        }
    }
}


void cd(const char* const path)
{
    long result;
    asm volatile("movq %1, %%rdi\n"
                 "movl $80, %%eax\n"
                 "syscall\n"
                 "movq %%rax, %0\n"
                 : "=r"(result)
                 : "r"(path)
                 : "%rdi", "%rax");

    // Set errno and handle errors if the system call failed
    if (result < 0)
    {
        errno = -result;
        perror("change_cwd_asm");
        exit(EXIT_FAILURE);
    }
}


void clib_mkdir(const char* const path)
{
    long result;
    asm volatile("movq %1, %%rdi\n"
                 "movl $83, %%eax\n"
                 "syscall\n"
                 "movq %%rax, %0\n"
                 : "=r"(result)
                 : "r"(path)
                 : "%rdi", "%rax");

    // Set errno and handle errors if the system call failed
    if (result < 0)
    {
        errno = -result;
        perror("make_directory");
    }
}


void get_cwd(char* cwd, size_t const size)
{
    long result;
    asm volatile("movq %1, %%rdi\n"
                 "movq %2, %%rsi\n"
                 "movl $79, %%eax\n"
                 "syscall\n"
                 "movq %%rax, %0\n"
                 : "=r"(result)
                 : "r"(cwd), "r"(size)
                 : "%rdi", "%rsi", "%rax");

    // Set errno and handle errors if the system call failed
    if (result < 0)
    {
        errno = -result;
        perror("get_cwd_asm");
        cwd = NULL;
    }
}


void get_user(char* user, size_t const size)
{
    uid_t uid;
    long  result;

    // Retrieve the user ID using inline assembly for getuid syscall
    asm volatile("movl $102, %%eax\n" // Syscall number for getuid (102) into EAX
                 "syscall\n"          // Invoke syscall
                 "movl %%eax, %0\n"   // Move the result from EAX to uid
                 : "=r"(uid)          // Output operand
                 :                    // No input operands
                 : "%eax"             // Clobbered register
    );

    if (uid < 0)
    {
        perror("get_user (getuid)");
        user = NULL;
        return;
    }

    struct passwd* pwd = getpwuid(uid);
    if (pwd == NULL)
    {
        perror("get_user (getpwuid)");
        user = NULL;
        return;
    }

    if (snprintf(user, size, "%s", pwd->pw_name) >= size)
    {
        fprintf(stderr, "get_user: buffer too small\n");
        user = NULL;
        return;
    }
}


void clear_buffer(char* const buffer, size_t const size)
{
    for (size_t i = 0; i < size; ++i)
    {
        ((char*)buffer)[i] = '\0';
    }
}


void rmdir_asm(const char* const path)
{
    long result;
    asm volatile("movq %1, %%rdi\n"  // Move the path argument to the first argument register (rdi)
                 "movl $84, %%eax\n" // Move the syscall number for rmdir to eax
                 "syscall\n"         // Invoke the syscall
                 "movq %%rax, %0\n"  // Move the syscall result to the result variable
                 : "=r"(result)
                 : "r"(path)
                 : "%rdi", "%rax");

    // Set errno and handle errors if the system call failed
    if (result < 0)
    {
        errno = -result;
    }
}


u8 clib_dir_exists(const char* const path)
{
    struct stat st;
    long        result;
    asm volatile("movq %1, %%rdi\n" // Move the path argument to the first argument register (rdi)
                 "movq %2, %%rsi\n" // Move the stat buffer to the second argument register (rsi)
                 "movl $4, %%eax\n" // Move the syscall number for newfstatat to eax
                 "syscall\n"        // Invoke the syscall
                 "movq %%rax, %0\n" // Move the syscall result to the result variable
                 : "=r"(result)
                 : "r"(path), "r"(&st)
                 : "%rdi", "%rsi", "%rax");

    // Check if the syscall succeeded
    if (result == 0)
    {
        // Check if it's a directory
        if (S_ISDIR(st.st_mode))
        {
            return DIRECTORY_EXISTS;
        }
        else
        {
            errno = ENOTDIR;
            return IS_NOT_A_DIRECTORY;
        }
    }
    else
    {
        errno = -result;
        return DOES_NOT_EXIST; // Directory does not exist
    }
}


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

u8 append_str(char* dest, const char* src, size_t dest_size)
{
    // Check if either pointer is null
    if (dest == NULL || src == NULL)
    {
        errno = EINVAL;
        perror("append_str");
        return NULL_FAILURE;
    }

    // Calculate the remaining buffer size
    size_t dest_len = 0;
    size_t src_len  = 0;

    asm volatile("1: "
                 "cmpb $0, (%0)\n" // Compare the byte at the current dest pointer with null character
                 "je 2f\n"         // If it's null, jump to label 2
                 "inc %0\n"        // Move to the next byte of dest
                 "inc %1\n"        // Increment destination length
                 "jmp 1b\n"        // Repeat the loop
                 "2:\n"
                 : "+r"(dest), "+r"(dest_len) // Output and input: updating the dest pointer and length
    );

    // Calculate the length of the source string
    asm volatile("3: "
                 "cmpb $0, (%0)\n" // Compare the byte at the current src pointer with null character
                 "je 4f\n"         // If it's null, jump to label 4
                 "inc %0\n"        // Move to the next byte of src
                 "inc %1\n"        // Increment source length
                 "jmp 3b\n"        // Repeat the loop
                 "4:\n"
                 : "+r"(src), "+r"(src_len) // Output and input: updating the src pointer and length
    );

    // Check if there is enough space in the destination buffer
    if (dest_len + src_len + 1 > dest_size)
    {
        errno = ENOMEM;
        perror("append_str");
        return FAILURE;
    }

    // Restore dest and src pointers to original positions
    dest -= dest_len;
    src -= src_len;

    // Finding the end of the destination string again (needed after pointer adjustment)
    asm volatile("1: "
                 "cmpb $0, (%0)\n" // Compare the byte at the current dest pointer with null character
                 "je 2f\n"         // If it's null, jump to label 2
                 "inc %0\n"        // Move to the next byte of dest
                 "jmp 1b\n"        // Repeat the loop
                 "2:\n"
                 : "+r"(dest) // Output and input: updating the dest pointer
    );

    // Appending the source string to the destination string
    asm volatile("3: "
                 "movb (%1), %%al\n" // Load the byte from src into al
                 "cmpb $0, %%al\n"   // Compare it with null character
                 "je 4f\n"           // If it's null, jump to label 4
                 "movb %%al, (%0)\n" // Store the byte in dest
                 "inc %0\n"          // Move to the next byte of dest
                 "inc %1\n"          // Move to the next byte of src
                 "jmp 3b\n"          // Repeat the loop
                 "4: "
                 "movb $0, (%0)\n"       // Null-terminate the resulting string
                 : "+r"(dest), "+r"(src) // Output and input: updating dest and src pointers
                 :
                 : "%al" // Clobber list: using al register
    );

    return SUCCESS;
}
