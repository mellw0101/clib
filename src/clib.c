#include "clib.h"

#include <sys/stat.h>
#include "def.h"


#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include "def.h"
#include "errno.h"
#include "pwd.h"
#include "stdio.h"
#include "string.h"


void read_from_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("fopen");
        clib_exit(EXIT_FAILURE);
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


size_t find(const char* const s, const char c)
{
    if (s == NULL || c == '\0')
    {
        errno = EINVAL;
        perror("find");
        return (size_t)-1;
    }


    for (size_t i = 0; s[i]; ++i)
    {
        if (s[i] == c)
            return i;
    }

    return (size_t)-1;
}


u8 cd(const char* const path)
{
    if (path == NULL)
    {
        errno = EINVAL;
        perror("cd");
        return NULL_FAILURE;
    }

    if (slen(path) == 0)
    {
        errno = EINVAL;
        perror("cd");
        return FAILURE;
    }

    size_t pos = find(path, '/');
    if (pos == (size_t)-1)
    {
        u8 append_result = 0;

        char full_path[256];
        get_cwd(full_path, 256);
        append_result = append_str(full_path, (const char* const)"/", 256);
        if (append_result == FAILURE || append_result == NULL_FAILURE)
        {
            perror("cd");
            return FAILURE;
        }

        const char* const cwd = full_path;


        assert(slen(full_path) < 256);

        long result;
        asm volatile("movq %1, %%rdi\n"
                     "movl $80, %%eax\n"
                     "syscall\n"
                     "movq %%rax, %0\n"
                     : "=r"(result)
                     : "r"(full_path)
                     : "%rdi", "%rax");

        if (result < 0)
        {
            errno = -result;
            perror("cd");
            return FAILURE;
        }

        return SUCCESS;
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

    long result = 0;
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


u8 get_cwd(void* buf, size_t const size)
{
    if (buf == NULL)
    {
        errno = EINVAL;
        perror("get_cwd");
        return NULL_FAILURE;
    }

    long result;
    __asm__ volatile("movq %1, %%rdi\n"
                     "movq %2, %%rsi\n"
                     "movl $79, %%eax\n"
                     "syscall\n"
                     "movq %%rax, %0\n"
                     : "=r"(result)
                     : "r"((char*)buf), "r"(size)
                     : "%rdi", "%rsi", "%rax");

    // Set errno and handle errors if the system call failed
    if (result < 0)
    {
        errno = -result;
        perror("get_cwd_asm");
        buf = NULL;
        return FAILURE;
    }

    return SUCCESS;
}


u8 get_user(void* __buf, size_t const __size)
{
    if (__buf == NULL)
    {
        errno = EINVAL;
        perror("get_user");
        return NULL_FAILURE;
    }

    uid_t uid    = 0;
    long  result = 0;

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
        __buf = NULL;
        return FAILURE;
    }

    struct passwd* pwd = getpwuid(uid);
    if (pwd == NULL)
    {
        perror("get_user (getpwuid)");
        __buf = NULL;
        return FAILURE;
    }

    if (snprintf(__buf, __size, "%s", pwd->pw_name) >= __size)
    {
        fprintf(stderr, "get_user: buffer too small\n");
        __buf = NULL;
        return FAILURE;
    }

    return SUCCESS;
}


void clear_buffer(const void* const buf, size_t const size)
{
    for (size_t i = 0; i < size; ++i)
    {
        ((char* const)buf)[i] = '\0';
    }
}


u8 clib_rm(const char* const path)
{
    if (path == NULL)
    {
        errno = EINVAL;
        __asm__call(perror, "clib_rm");
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
        perror("clib_rm");
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

#include <sys/stat.h>
#include <sys/types.h>


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

ssize_t clib_read(int fd, const void* buf, size_t count)
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

void clib_exit(int status)
{
    asm volatile("mov $60, %%rax\n" // syscall number for exit
                 "mov %0, %%rdi\n"  // exit status
                 "syscall\n"        // invoke syscall
                 :
                 : "r"((u64)status) // make sure the status is passed as a 64-bit value
                 : "%rax", "%rdi");

    __builtin_unreachable(); // Ensure the compiler knows this code is unreachable
}


int clib_dup2(int oldfd, int newfd)
{
    int result;

    // Inline assembly to call the dup2 system call
    __asm__ volatile("syscall"
                     : "=a"(result) // Output operand list
                     : "a"(33),     // Input operands list (syscall number for dup2 is 33 on x86_64)
                       "D"(oldfd), "S"(newfd)
                     : "rcx", "r11", "memory" // Clobbered registers
    );

    // Set errno if the syscall returns an error
    if (result < 0)
    {
        errno  = -result;
        result = DUP2_FAILURE;
    }

    return result;
}

void abort(void)
{
    asm volatile("mov $60, %%rax\n" // syscall number for exit (60)
                 "mov $1, %%rdi\n"  // exit status (1 indicates failure)
                 "syscall\n"        // invoke syscall
                 :
                 :
                 : "%rax", "%rdi");

    // In case the syscall fails, ensure the program still terminates
    __builtin_unreachable(); // Indicate to the compiler that this point is never reached
}

pid_t clib_waitpid(pid_t pid, int* status, int options)
{
    pid_t result;

    __asm__ volatile("movl %2, %%ebx\n"
                     "movl %4, %%edx\n"
                     "movl $7, %%eax\n"
                     "int $0x80\n"
                     "movl %%eax, %0\n"
                     : "=r"(result)
                     : "0"(result), "r"(pid), "r"(status), "r"(options)
                     : "%eax", "%ebx", "%ecx", "%edx");

    if (result == -1)
    {
        errno  = -result;
        result = -1;
    }

    return result;
}

void launch_binary(const char* const path, char* const* argv, char* const* envp)
{
    __asm__ volatile("mov %0, %%rdi\n"  // path
                     "mov %1, %%rsi\n"  // argv
                     "mov %2, %%rdx\n"  // envp
                     "mov $59, %%rax\n" // syscall number for execve
                     "syscall\n"
                     : // No output operands (no return value) __builtin_unreachable();
                     : "r"(path), "r"(argv), "r"(envp)
                     : "%rdi", "%rsi", "%rdx", "%rax");

    // Ensure the compiler knows this code is unreachable
    __builtin_unreachable();
}


int clib_free(void* ptr, size_t const size)
{
    if (ptr == NULL)
    {
        errno = EINVAL;
        perror("clib_free: (ptr == NULL)");
        return NULL_FAILURE;
    }

    long result = (long)-1;
    __asm__ volatile("movq %1, %%rdi\n"  // Set first argument to ptr
                     "movq %2, %%rsi\n"  // Set second argument to size
                     "movl $11, %%eax\n" // Set syscall number for munmap (11)
                     "syscall\n"         // Make syscall
                     "movq %%rax, %0\n"  // Store return value in result
                     : "=r"(result)
                     : "r"(ptr), "r"(size)
                     : "%rdi", "%rsi", "%rax");

    if (result < 0)
    {
        errno = -result;
        perror("munmap");
        return FAILURE;
    }

    return SUCCESS;
}

size_t get_std_str_len(const void* s)
{
    if ((char*)s == NULL)
        return (size_t)-1;

    size_t i = (size_t)0;
    for (; &s[i]; ++i)
        ;

    return (size_t)i;
}


u8 fork_and_launch_binary(const char* const path, char* const* const argv, char* const* const envp)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return FAILURE;
    }

    if (pid == 0)
    {
        launch_binary(path, argv, envp);
    }
    else
    {
        clib_waitpid(pid, NULL, 0);
    }

    return SUCCESS;
}


u8 printf_ls_cwd(void)
{
    char* cwd = malloc(std_str_len);
    get_cwd(cwd, std_str_len);
    printf("cwd = %s\n", cwd);

    char* const argv[3] = {"/usr/bin/ls", cwd, NULL};

    u8 result = fork_and_launch_binary("/usr/bin/ls", argv, NULL);

    return SUCCESS;
}


#define META_SIZE sizeof(struct block_meta)


void* global_base = NULL;


// Find a free block or extend the heap if necessary
struct block_meta* find_free_block(struct block_meta** last, size_t size)
{
    struct block_meta* current = global_base;
    while (current && !(current->free && current->size >= size))
    {
        *last   = current;
        current = current->next;
    }
    return current;
}


// Request more memory from the system
struct block_meta* request_space(struct block_meta* last, size_t size)
{
    struct block_meta* block;
    block = sbrk(size + META_SIZE);
    if (block == (void*)-1) // sbrk failed
    {
        return NULL;
    }

    if (last) // Not the first block
    {
        last->next = block;
    }

    block->size = size;
    block->next = NULL;
    block->free = 0;
    return block;
}


// Align size to multiple of 8 for better memory alignment
size_t align_size(size_t size)
{
    return (size + 7) & ~7;
}


// Allocate memory
void* malloc(size_t size)
{
    struct block_meta* block;

    if (size <= 0)
    {
        return NULL;
    }

    size = align_size(size);

    if (!global_base) // First call
    {
        block = request_space(NULL, size);
        if (!block)
        {
            return NULL;
        }
        global_base = block;
    }
    else
    {
        struct block_meta* last = global_base;
        block                   = find_free_block(&last, size);
        if (!block) // Failed to find a free block
        {
            block = request_space(last, size);
            if (!block)
            {
                return NULL;
            }
        }
        else // Found a free block
        {
            block->free = 0;
        }
    }

    return (block + 1);
}


// Free memory
void free(void* ptr)
{
    if (!ptr)
    {
        return;
    }

    struct block_meta* block_ptr = (struct block_meta*)ptr - 1;
    block_ptr->free              = 1;

    // Merge adjacent free blocks
    struct block_meta* current = global_base;
    while (current)
    {
        if (current->free && current->next && current->next->free)
        {
            current->size += current->next->size + META_SIZE;
            current->next = current->next->next;
        }
        current = current->next;
    }
}


// Reallocate memory
void* realloc(void* ptr, size_t size)
{
    if (!ptr)
    {
        return malloc(size);
    }

    struct block_meta* block_ptr = (struct block_meta*)ptr - 1;
    if (block_ptr->size >= size)
    {
        return ptr;
    }

    void* new_ptr = malloc(size);
    if (new_ptr)
    {
        memcpy(new_ptr, ptr, block_ptr->size);
        free(ptr);
    }

    return new_ptr;
}


// Allocate memory for an array
void* calloc(size_t num, size_t size)
{
    size_t total_size = num * size;
    void*  ptr        = malloc(total_size);
    if (ptr)
    {
        memset(ptr, 0, total_size);
    }
    return ptr;
}


// Define the structure
typedef std_str_t  std_str_t;
typedef std_str_t* std_str;

// Function to convert std_str_t to void*
std_str std_str_to_void_ptr(char* data)
{
    std_str new_str = (std_str)malloc(sizeof(std_str_t));
    if (!new_str)
    {
        return NULL;
    }

    new_str->data = data;

    return new_str;
}

std_str std_str_new(char* const data)
{
    std_str_t* new_str = (std_str)malloc(sizeof(std_str_t));
    if (!new_str)
    {
        return NULL;
    }

    new_str->data = malloc(slen(data) + 1);
    if (!new_str->data)
    {
        free(new_str);
        return NULL;
    }


    strcp(new_str->data, data);
    return new_str;
}

// Function to convert std_str_t to char*
static char* std_str_to_char_ptr(std_str_t* self)
{
    return (char*)self->data;
}


// Function to free the structure
void std_str_free(std_str str)
{
    if (str)
    {
        free(str->data);
        free(str);
    }
}

// Function to initialize the structure
std_str_t* init_std_str_t(const char* data);

// Function to free the structure
void free_std_str_t(std_str_t* str);

// Function to print the string data
void print_std_str_t(const std_str_t* str);

// Function to initialize the structure
std_str_t* init_std_str_t(const char* data)
{
    std_str_t* new_str = (std_str_t*)malloc(sizeof(std_str_t));
    if (!new_str)
    {
        return NULL;
    }

    new_str->data = malloc(slen(data) + 1);
    if (!new_str->data)
    {
        free(new_str);
        return NULL;
    }

    strcp(new_str->data, data);
    return new_str;
}

// Function to free the structure
void free_std_str_t(std_str_t* str)
{
    if (str)
    {
        free(str->data);
        free(str);
    }
}

// Function to print the string data
void print_std_str_t(const std_str_t* str)
{
    if (str && str->data)
    {
        printf("%s\n", (char*)str->data);
    }
}