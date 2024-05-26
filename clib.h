#ifndef CLIB_H
#define CLIB_H
/**

    @file clib.h
    @brief Header file for the clib library.

    This header file contains declarations for functions and types provided by the clib library.
    The clib library provides various utility functions for file I/O, string manipulation, directory
    operations, and more.

*/

#include <stddef.h>
#include <stdio.h>
#include "def.h"


// #define __VA_ARGS__SIZE__ (sizeof((i32[]){__VA_ARGS__}) / sizeof(i32))


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

/**

    @brief Reads data from a file and prints it to the console.

    This function reads the contents of the file specified by the given filename and prints it to the console.

    @param filename The name of the file to read from.

*/
void read_from_file(const char* filename);

u8 clib_fputs(const char* restrict s, FILE* restrict stream);


/**

    @brief Writes data to a file.

    This function writes the given data to the file specified by the given filename.

    @param filename The name of the file to write to.
    @param data The data to write to the file.

*/
u8 write_to_file(const char* filename, const char* data);


/**

    @brief Copies a string from source to destination.

    This function copies the null-terminated string from the source to the destination.

    @param dest The destination buffer to copy the string to.
    @param src The source string to copy from.

*/
u8 strcp(char* dest, const char* src);


/**

    @brief Calculates the length of a string.

    This function calculates the length of the null-terminated string.

    @param s The string to calculate the length of.
    @return The length of the string.

*/
size_t slen(const char* s);


/**

    @brief Changes the current working directory.

    This function changes the current working directory to the specified path.

    @param path The path to change the current working directory to.

*/
u8 cd(const char* path);


/**

    @brief Creates a new directory.

    This function creates a new directory with the specified path.

    @param path The path of the directory to create.

*/
u8 clib_mkdir(const char* path);


/**

    @brief Retrieves the current working directory.

    This function retrieves the current working directory and stores it in the specified buffer.

    @param cwd The buffer to store the current working directory in.
    @param size The size of the buffer.

*/
u8 get_cwd(void* buf, size_t size);


/**

    @brief Retrieves the username.

    This function retrieves the username and stores it in the specified buffer.

    @param user The buffer to store the username in.
    @param size The size of the buffer.

*/
u8 get_user(void* buf, size_t buf_size);


/**

    @brief Clears the contents of a buffer.

    This function clears the contents of the specified buffer by filling it with null characters.

    @param buffer The buffer to clear.
    @param size The size of the buffer.

*/
void clear_buffer(const void* buf, size_t size);


u8 clib_rm(const char* path);


/**

    @brief Removes a directory and its contents.

    This function removes the directory specified by the given path, including all its contents.

    @param path The path of the directory to remove.

*/
u8 clib_rmdir(const char* path);


/**

    @brief Checks if a directory exists.

    This function checks if the directory specified by the given path exists.

    @param path The path of the directory to check.

    @returns @param DIRECTORY_EXISTS if @param path is a directory and exists,
    @returns @param IS_NOT_A_DIRECTORY if @param path is not a directory,
    @returns @param DOES_NOT_EXIST if @param path does not exist.

*/
u8 clib_dir_exists(const char* path);


/**

    @brief Appends a string to another string.

    This function appends the source string to the destination string.

    @param dest The destination string to append to.
    @param src The source string to append.
    @param dest_size The size of the destination buffer.
    @returns SUCCESS on successfull append,
    @returns FAILUER on failure to append,
    @returns NULL_FAILUER on ether @param dest = NULL or @param src = NULL.

*/
u8 append_str(char* dest, const char* src, size_t dest_size);


/**

    @brief Allocates memory.

    This function allocates memory of the specified size.

    @param size The size of the memory to allocate.
    @return A pointer to the allocated memory.

*/
void* allocate(size_t const size);


int clib_connect(int sockfd, const struct sockaddr* addr, __socklen_t addrlen);

#ifndef assert
    #define assert(condition) ((condition) ? (void)0 : assert_statement(#condition, __FILE__, __LINE__))
#else
    #define clib_assert(condition) ((condition) ? (void)0 : assert_statement(#condition, __FILE__, __LINE__))
#endif

void assert_statement(const char* condition, const char* file, int line);


void clib_exit(int __status) __attribute__((__noreturn__));

/**

    @brief Duplicates a file descriptor to another file descriptor.

    This function duplicates the file descriptor `oldfd` to `newfd`.
    If `newfd` is already open, it will be closed first.

    @param oldfd The file descriptor to duplicate.
    @param newfd The file descriptor to duplicate to.
    @return On success, the new file descriptor is returned.
    @returns @param DUP2_FAILURE On failure
    GLOBAL: @param errno is set appropriately.

*/
int clib_dup2(int oldfd, int newfd);


void abort(void) __attribute__((__noreturn__));


ssize_t clib_read(int fd, const void* buf, size_t count);

void launch_binary(const char* path, char* const argv[], char* const envp[]) __attribute__((__noreturn__));

u8 printf_ls_cwd(void);

size_t find(const char*, const char);

pid_t clib_waitpid(pid_t, int*, int);

int clib_free(void*, size_t);
u8  fork_and_launch_binary(const char* path, char* const* argv, char* const* envp);


// #define __VA_ARGS__SIZE__ (sizeof((i32[]){__VA_ARGS__}) / sizeof(i32))

void* clib_memcpy(void* dest, const void* src, size_t n);

#ifndef MALLOC_H
    #define MALLOC_H

    #include <stddef.h>

struct block_meta
{
    size_t             size;
    struct block_meta* next;
    int                free;
};

void* global_base;

static struct block_meta* find_free_block(struct block_meta** last, size_t size);
static struct block_meta* request_space(struct block_meta* last, size_t size);
static size_t             align_size(size_t size);
void*                     malloc(size_t size);
void                      free(void* ptr);
void*                     realloc(void* ptr, size_t size);
void*                     calloc(size_t num, size_t size);

#endif


#ifndef __std_str_h__
    #define __std_str_h__

// Define the structure
typedef struct std_str_t std_str_t;
typedef std_str_t        std_str_t;

struct std_str_t
{
    void* data;
    void* (*constructor)(void* self);
};

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

static std_str_t* init_std_str_t(const char* data);

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


#endif // __std_str_h__


#endif
