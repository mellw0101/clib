#pragma once

#ifndef __DEF_H__
    #define __DEF_H__

/* Convenience types.  */
typedef unsigned char      __u_char;
typedef unsigned short int __u_short;
typedef unsigned int       __u_int;
typedef unsigned long int  __u_long;

/* Fixed-size types, underlying types depend on word size and compiler.  */
typedef signed char      __int8_t;
typedef signed short int __int16_t;
typedef signed int       __int32_t;

typedef unsigned char      __uint8_t;
typedef unsigned short int __uint16_t;
typedef unsigned int       __uint32_t;

    #if __WORDSIZE == 64
typedef signed long int   __int64_t;
typedef unsigned long int __uint64_t;
    #else
__extension__ typedef signed long long int   __int64_t;
__extension__ typedef unsigned long long int __uint64_t;
    #endif

/* Smallest types with at least a given width.  */
typedef __int8_t  __int_least8_t;
typedef __int16_t __int_least16_t;
typedef __int32_t __int_least32_t;
typedef __int64_t __int_least64_t;

typedef __uint8_t  __uint_least8_t;
typedef __uint16_t __uint_least16_t;
typedef __uint32_t __uint_least32_t;
typedef __uint64_t __uint_least64_t;

/* quad_t is also 64 bits.  */
    #if __WORDSIZE == 64
typedef long int          __quad_t;
typedef unsigned long int __u_quad_t;
    #else
__extension__ typedef long long int          __quad_t;
__extension__ typedef unsigned long long int __u_quad_t;
    #endif

typedef __int_least64_t i64_least_t;
typedef __int_least32_t i32_least_t;
typedef __int_least16_t i16_least_t;
typedef __int_least8_t  i8_least_t;

typedef __uint_least64_t u64_least_t;
typedef __uint_least32_t u32_least_t;
typedef __uint_least16_t u16_least_t;
typedef __uint_least8_t  u8_least_t;

typedef __int64_t i64;
typedef __int32_t i32;
typedef __int16_t i16;
typedef __int8_t  i8;

typedef __uint64_t u64;
typedef __uint32_t u32;
typedef __uint16_t u16;
typedef __uint8_t  u8;

typedef u64 size_t;
typedef i64 ssize_t;

typedef unsigned int __socklen_;


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


typedef unsigned short sa_family_t;

typedef long int __off_t;
    #ifndef __off64_t
        #define __off64_t long long int
    #endif
struct sockaddr
{
    sa_family_t sa_family;   // Address family (e.g., AF_INET for IPv4)
    char        sa_data[14]; // Address data (14 bytes for compatibility)
};

#endif //__DEF_H__
