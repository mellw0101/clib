#include "clib.h"

#include "def.h"
#include "errno.h"
#include "pwd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "struct_FILE.h"
#include "sys/stat.h"

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

u8 clib_fputs(const char* restrict s, FILE* restrict stream)
{
    if (s == NULL || stream == NULL)
    {
        errno = EINVAL;
        perror("fputs_asm");
        return NULL_FAILURE;
    }

    size_t len = 0;
    while (s[len] != '\0')
    {
        len++;
    }

    // Get the file descriptor from the FILE* stream
    int fd = fileno(stream);
    if (fd < 0)
    {
        perror("fileno");
        return FAILURE;
    }

    ssize_t result;
    asm volatile("movq $1, %%rax\n" // syscall number for write
                 "movq %1, %%rdi\n" // file descriptor
                 "movq %2, %%rsi\n" // buffer (pointer to string)
                 "movq %3, %%rdx\n" // count (length of string)
                 "syscall\n"        // invoke syscall
                 "movq %%rax, %0\n" // store result in variable
                 : "=r"(result)
                 : "r"((long)fd), "r"(s), "r"((long)len)
                 : "%rax", "%rdi", "%rsi", "%rdx");

    if (result < 0)
    {
        errno = -result;
        perror("fputs_asm");
        return FAILURE;
    }

    return SUCCESS;
}


u8 write_to_file(const char* filename, const char* data)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        errno = EINVAL;
        perror("fopen");
        return NULL_FAILURE;
    }

    u8 result = clib_fputs(data, file);
    fclose(file);

    if (result == FAILURE)
    {
        perror("write_to_file");
        return FAILURE;
    }

    return SUCCESS;
}


size_t slen(const char* s)
{
    if (s == NULL)
    {
        errno = EINVAL;
        perror("slen");
        return (size_t)-1;
    }

    size_t i = 0;
    for (; s[i]; ++i)
        ;

    return i;
}


u8 strcp(char* dest, const char* src)
{
    if (dest == NULL || src == NULL)
    {
        errno = EINVAL;
        perror("strcp");
        return NULL_FAILURE;
    }

    size_t i = 0;
    for (; src[i]; ++i)
        dest[i] = src[i];

    dest[i] = '\0';

    if (i != slen(src))
    {
        errno = ENOMEM;
        perror("strcp");
        return FAILURE;
    }

    return SUCCESS;
}


u8 cd(const char* const path)
{
    if (path == NULL)
    {
        errno = EINVAL;
        perror("cd");
        return NULL_FAILURE;
    }

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
        perror("cd");
        return FAILURE;
    }

    return SUCCESS;
}


u8 clib_mkdir(const char* const path)
{
    if (path == NULL)
    {
        errno = EINVAL;
        perror("clib_mkdir");
        return NULL_FAILURE;
    }

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
        perror("clib_mkdir");
        return FAILURE;
    }

    return SUCCESS;
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

u8 clib_rm(const char* const path)
{
    if (path == NULL)
    {
        errno = EINVAL;
        perror("clib_unlink");
        return NULL_FAILURE;
    }

    long result;
    asm volatile("movq %1, %%rdi\n"  // Move the path argument to the first argument register (rdi)
                 "movl $87, %%eax\n" // Move the syscall number for unlink (87) to eax
                 "syscall\n"         // Invoke the syscall
                 "movq %%rax, %0\n"  // Move the syscall result to the result variable
                 : "=r"(result)
                 : "r"(path)
                 : "%rdi", "%rax");

    // Set errno and handle errors if the system call failed
    if (result < 0)
    {
        errno = -result;
        perror("clib_unlink");
        return FAILURE;
    }

    return SUCCESS;
}


u8 clib_rmdir(const char* const path)
{
    if (path == NULL)
    {
        errno = EINVAL;
        perror("clib_rmdir");
        return NULL_FAILURE;
    }

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
        perror("clib_rmdir");
        return FAILURE;
    }

    return SUCCESS;
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


void* allocate(size_t const size)
{
    void* result;
    long  mmap_result;

    asm volatile("movq $9, %%rax\n"  // syscall number for mmap
                 "movq $0, %%rdi\n"  // addr (NULL -> kernel chooses the address)
                 "movq %1, %%rsi\n"  // length
                 "movq $3, %%rdx\n"  // prot (PROT_READ | PROT_WRITE)
                 "movq $34, %%r10\n" // flags (MAP_PRIVATE | MAP_ANONYMOUS)
                 "movq $-1, %%r8\n"  // fd (-1 for anonymous mapping)
                 "movq $0, %%r9\n"   // offset (0 for anonymous mapping)
                 "syscall\n"         // invoke syscall
                 "movq %%rax, %0\n"  // store result in mmap_result
                 : "=r"(mmap_result)
                 : "r"(size)
                 : "%rax", "%rdi", "%rsi", "%rdx", "%r10", "%r8", "%r9");

    if (mmap_result == -1)
    {
        errno = -mmap_result;
        perror("allocate");
        return NULL;
    }

    result = (void*)mmap_result;
    return result;
}


int clib_connect(int sockfd, const struct sockaddr* addr, __socklen_t addrlen)
{
    if (addr == NULL)
    {
        errno = EINVAL;
        perror("connect_asm");
        return NULL_FAILURE;
    }

    long result;
    asm volatile("movq $42, %%rax\n" // syscall number for connect
                 "movq %1, %%rdi\n"  // sockfd
                 "movq %2, %%rsi\n"  // addr
                 "movq %3, %%rdx\n"  // addrlen
                 "syscall\n"         // invoke syscall
                 "movq %%rax, %0\n"  // store result in variable
                 : "=r"(result)
                 : "r"((long)sockfd), "r"(addr), "r"((long)addrlen)
                 : "%rax", "%rdi", "%rsi", "%rdx");

    if (result < 0)
    {
        errno = -result;
        perror("connect_asm");
        return FAILURE;
    }

    return SUCCESS;
}


void assert_statement(const char* condition, const char* file, int line)
{
    fprintf(stderr, "Assertion failed: (%s), file %s, line %d.\n", condition, file, line);

    // Inline assembly to invoke a system call to abort the program
    asm volatile("movq $60, %%rax\n" // syscall number for exit (60)
                 "movq $1, %%rdi\n"  // exit status (1 indicates failure)
                 "syscall\n"         // invoke syscall
                 :
                 :
                 : "%rax", "%rdi");

    // In case the syscall fails, ensure the program still terminates
    abort();
}

ssize_t asm_read(int fd, void* buf, size_t count)
{
    ssize_t result;
    asm volatile("movq $0, %%rax\n" // syscall number for read (0)
                 "movq %1, %%rdi\n" // file descriptor
                 "movq %2, %%rsi\n" // buffer
                 "movq %3, %%rdx\n" // count
                 "syscall\n"        // invoke syscall
                 "movq %%rax, %0\n" // store result in variable
                 : "=r"(result)
                 : "r"((long)fd), "r"(buf), "r"((long)count)
                 : "%rax", "%rdi", "%rsi", "%rdx");

    return result;
}


// Define the fileno_asm function
// int fileno_asm(FILE* file)
// {
//     if (file == NULL)
//     {
//         errno = EINVAL;
//         perror("fileno_asm");
//         return -1;
//     }

//     int fd;
//     asm volatile("movq (%1), %0\n" // Move the value at the address of file (the file descriptor) into fd
//                  : "=r"(fd)        // Output operand
//                  : "r"(file)       // Input operand
//     );

//     return fd;
// }
