#ifndef CLIB_H
#define CLIB_H

/**
 * @file clib.h
 * @brief Header file for the clib library.
 *
 * This header file contains declarations for functions and types provided by the clib library.
 * The clib library provides various utility functions for file I/O, string manipulation, directory operations, and
 * more.
 */

typedef unsigned long size_t;
typedef unsigned int  i32;


typedef unsigned long long u64;
typedef unsigned int       u32;
typedef unsigned int       u16;
typedef unsigned short     u8;

/**
 * @brief Reads data from a file and prints it to the console.
 *
 * This function reads the contents of the file specified by the given filename and prints it to the console.
 *
 * @param filename The name of the file to read from.
 */
void read_from_file(const char* filename);

/**
 * @brief Writes data to a file.
 *
 * This function writes the given data to the file specified by the given filename.
 *
 * @param filename The name of the file to write to.
 * @param data The data to write to the file.
 */
void write_to_file(const char* filename, const char* data);

/**
 * @brief Copies a string from source to destination.
 *
 * This function copies the null-terminated string from the source to the destination.
 *
 * @param dest The destination buffer to copy the string to.
 * @param src The source string to copy from.
 */
void strcp(char* dest, const char* src);

/**
 * @brief Calculates the length of a string.
 *
 * This function calculates the length of the null-terminated string.
 *
 * @param s The string to calculate the length of.
 * @return The length of the string.
 */
size_t slen(const char* s);

/**
 * @brief Structure to hold user information.
 */
typedef struct UserInfo_t
{
    char user[256]; ///< The username.
    char cwd[256];  ///< The current working directory.
} UserInfo;

/**
 * @brief Retrieves user information.
 *
 * This function retrieves the user information, including the username and the current working directory.
 *
 * @return A pointer to the UserInfo structure containing the user information.
 */
UserInfo* get_user_info(void);

/**
 * @brief Changes the current working directory.
 *
 * This function changes the current working directory to the specified path.
 *
 * @param path The path to change the current working directory to.
 */
void cd(const char* path);

/**
 * @brief Creates a new directory.
 *
 * This function creates a new directory with the specified path.
 *
 * @param path The path of the directory to create.
 */
void clib_mkdir(const char* path);

/**
 * @brief Retrieves the current working directory.
 *
 * This function retrieves the current working directory and stores it in the specified buffer.
 *
 * @param cwd The buffer to store the current working directory in.
 * @param size The size of the buffer.
 */
void get_cwd(char* cwd, size_t size);

/**
 * @brief Retrieves the username.
 *
 * This function retrieves the username and stores it in the specified buffer.
 *
 * @param user The buffer to store the username in.
 * @param size The size of the buffer.
 */
void get_user(char* user, size_t size);

/**
 * @brief Clears the contents of a buffer.
 *
 * This function clears the contents of the specified buffer by filling it with null characters.
 *
 * @param buffer The buffer to clear.
 * @param size The size of the buffer.
 */
void clear_buffer(char* buffer, size_t size);

/**
 * @brief Removes a directory and its contents.
 *
 * This function removes the directory specified by the given path, including all its contents.
 *
 * @param path The path of the directory to remove.
 */
void rmdir_asm(const char* path);

#ifndef DIRECTORY_EXISTS
    #define DIRECTORY_EXISTS 1
#else
    #undef DIRECTORY_EXISTS
    #define DIRECTORY_EXISTS 1
#endif

#ifndef IS_NOT_A_DIRECTORY
    #define IS_NOT_A_DIRECTORY 0
#else
    #undef IS_NOT_A_DIRECTORY
    #define IS_NOT_A_DIRECTORY 0
#endif

#ifndef DOES_NOT_EXIST
    #define DOES_NOT_EXIST 2
#else
    #undef DOES_NOT_EXIST
    #define DOES_NOT_EXIST 2
#endif

/**
 * @brief Checks if a directory exists.
 *
 * This function checks if the directory specified by the given path exists.
 *
 * @param path The path of the directory to check.
 * @return @param DIRECTORY_EXISTS if the directory exists, @param IS_NOT_A_DIRECTORY if it is not a directory,
 * or @param DOES_NOT_EXIST if it does not exist.
 */
u8 clib_dir_exists(const char* path);

#ifndef SUCCESS
    #define SUCCESS 0
#else
    #undef SUCCESS
    #define SUCCESS 0
#endif

#ifndef FAILURE
    #define FAILURE 1
#else
    #undef FAILURE
    #define FAILURE 1
#endif

#ifndef NULL_FAILURE
    #define NULL_FAILURE 2
#else
    #undef NULL_FAILURE
    #define NULL_FAILURE 2
#endif

u8 append_str(char* dest, const char* src, size_t dest_size);

#endif /* CLIB_H */
