#pragma once

#ifndef __DEF_H__
    #define __DEF_H__
    #ifndef __STD_TYPE
        #define __STD_TYPE typedef
    #endif
/* Convenience types.  */
__STD_TYPE unsigned char      __u_char;
__STD_TYPE unsigned short int __u_short;
__STD_TYPE unsigned int       __u_int;
__STD_TYPE unsigned long int  __u_long;

/* Fixed-size types, underlying types depend on word size and compiler.  */
__STD_TYPE signed char      __int8_t;
__STD_TYPE signed short int __int16_t;
__STD_TYPE signed int       __int32_t;

__STD_TYPE unsigned char      __uint8_t;
__STD_TYPE unsigned short int __uint16_t;
__STD_TYPE unsigned int       __uint32_t;

    #if __WORDSIZE == 64
__STD_TYPE signed long int   __int64_t;
__STD_TYPE unsigned long int __uint64_t;
    #else
__extension__ __STD_TYPE signed long long int   __int64_t;
__extension__ __STD_TYPE unsigned long long int __uint64_t;
    #endif

/* Smallest types with at least a given width.  */
__STD_TYPE __int8_t  __int_least8_t;
__STD_TYPE __int16_t __int_least16_t;
__STD_TYPE __int32_t __int_least32_t;
__STD_TYPE __int64_t __int_least64_t;

__STD_TYPE __uint8_t  __uint_least8_t;
__STD_TYPE __uint16_t __uint_least16_t;
__STD_TYPE __uint32_t __uint_least32_t;
__STD_TYPE __uint64_t __uint_least64_t;

/* quad_t is also 64 bits.  */
    #if __WORDSIZE == 64
__STD_TYPE long int          __quad_t;
__STD_TYPE unsigned long int __u_quad_t;
    #else
__extension__ __STD_TYPE long long int          __quad_t;
__extension__ __STD_TYPE unsigned long long int __u_quad_t;
    #endif

__STD_TYPE __int_least64_t i64_least_t;
__STD_TYPE __int_least32_t i32_least_t;
__STD_TYPE __int_least16_t i16_least_t;
__STD_TYPE __int_least8_t  i8_least_t;

__STD_TYPE __uint_least64_t u64_least_t;
__STD_TYPE __uint_least32_t u32_least_t;
__STD_TYPE __uint_least16_t u16_least_t;
__STD_TYPE __uint_least8_t  u8_least_t;

__STD_TYPE __int64_t i64;
__STD_TYPE __int32_t i32;
__STD_TYPE __int16_t i16;
__STD_TYPE __int8_t  i8;


__STD_TYPE __uint64_t u64;
__STD_TYPE __uint32_t u32;
__STD_TYPE __uint16_t u16;
__STD_TYPE __uint8_t  u8;


__STD_TYPE u64 uint64_t;
__STD_TYPE u32 uint32_t;
__STD_TYPE u16 uint16_t;
__STD_TYPE u8  uint8_t;


__STD_TYPE i64 int64_t;
__STD_TYPE i32 int32_t;
__STD_TYPE i16 int16_t;
__STD_TYPE i8  int8_t;


__STD_TYPE u64 size_t;
__STD_TYPE i64 ssize_t;

__STD_TYPE unsigned int __socklen_;

    #ifndef DUP2_FAILURE
        #define DUP2_FAILURE -1
    #else
        #undef DUP2_FAILURE
        #define DUP2_FAILURE -1
    #endif

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

    #ifndef DIRECTORY_EXISTS
        #define DIRECTORY_EXISTS 3
    #else
        #undef DIRECTORY_EXISTS
        #define DIRECTORY_EXISTS 3
    #endif

    #ifndef IS_NOT_A_DIRECTORY
        #define IS_NOT_A_DIRECTORY 4
    #else
        #undef IS_NOT_A_DIRECTORY
        #define IS_NOT_A_DIRECTORY 4
    #endif

    #ifndef DOES_NOT_EXIST
        #define DOES_NOT_EXIST 5
    #else
        #undef DOES_NOT_EXIST
        #define DOES_NOT_EXIST 5
    #endif

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

    #define EXIT_FAILURE 1
    #define EXIT_SUCCESS 0

    #define __S32_TYPE int
    #define __PID_T_TYPE __S32_TYPE

    #define __STD_TYPE typedef
__STD_TYPE __PID_T_TYPE __pid_t;

    #ifndef __pid_t_defined
typedef __pid_t pid_t;
        #define __pid_t_defined
    #endif

__STD_TYPE unsigned short sa_family_t;

    #define O_CREAT 0x40
    #define O_WRONLY 0x01

struct sockaddr
{
    sa_family_t sa_family;   // Address family (e.g., AF_INET for IPv4)
    char        sa_data[14]; // Address data (14 bytes for compatibility)
};

    // Define the static assertion macro
    #define STATIC_ASSERT(COND, MSG) __STD_TYPE char static_assertion_##MSG[(COND) ? 1 : -1]

// Helper macro to check for const qualifier at compile time
    #define IS_CONST_CHAR_PTR(T) _Generic((T), const char* const: 1, default: 0)

    #define MAX_PATH 260

    #define MAX_FILE_NAME 256

    #define MAX_FILE_EXT 4

    #define MAX_FILE_PATH (MAX_PATH + MAX_FILE_NAME + MAX_FILE_EXT)

    #define MAX_FILE_SIZE 1024

    #define MAX_FILE_MODE 0777

    #define MAX_BUFFER_SIZE 1024

    #define MAX_MSG_SIZE 1024

    #define MAX_PORT_SIZE 8

    #define MAX_PORT_NUM 65535

    #define MAX_IP_SIZE 16

    #define MAX_IPv4_SIZE 16

extern char** environ;

    #define std_str_len 256

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

    #define NULL ((void*)0)

#endif //__DEF_H__
