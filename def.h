#pragma once

#ifndef __DEF_H__
    #define __DEF_H__

typedef unsigned long size_t;
typedef unsigned int  i32;


typedef unsigned long long u64;
typedef unsigned int       u32;
typedef unsigned int       u16;
typedef unsigned short     u8;

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
#endif //__DEF_H__
