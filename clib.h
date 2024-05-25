#ifndef CLIB_H
#define CLIB_H
/**

    @file clib.h
    @brief Header file for the clib library.

    This header file contains declarations for functions and types provided by the clib library.
    The clib library provides various utility functions for file I/O, string manipulation, directory
    operations, and more.

*/

#include "def.h"
#include "stdio.h"

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
void get_cwd(char* cwd, size_t size);


/**

    @brief Retrieves the username.

    This function retrieves the username and stores it in the specified buffer.

    @param user The buffer to store the username in.
    @param size The size of the buffer.

*/
void get_user(char* user, size_t size);


/**

    @brief Clears the contents of a buffer.

    This function clears the contents of the specified buffer by filling it with null characters.

    @param buffer The buffer to clear.
    @param size The size of the buffer.

*/
void clear_buffer(char* buffer, size_t size);


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


#endif /* CLIB_H */
