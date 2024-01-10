#pragma once

/**
 * @file rdefines.h
 * @author Radu-D. Chira
 * @brief Defines I use the most while writing C
 * @version 0.1
 * @date 2024-01-10
 */

#include <stdbool.h>
#include <assert.h>

// Platforms defines
#if defined(WIN32) || defined(_WIN32) || (__WIN32__)
    #define RPLATFORM_WINDOWS 1
#elif defined(__linux__) || (__gnu_linux__)
    #define RPLATFORM_LINUX 1
#elif defined(__unix__)
    #define RPLATFORM_UNIX 1
#elif defined(__APPLE__)
    #define RPLATFORM_APPLE 1
#else
    #error "Unsupported"
#endif

#ifdef _MSC_VER
    #define DEBUGBREAK() __debugbreak()
#else
    #define DEBUGBREAK() 
#endif

#ifdef REXPORT
    #ifdef _MSC_VER
        #define RAPI __declspec(dllexport)
    #else
        #define RAPI __attribute__((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
        #define RAPI __declspec(dllimport)
    #else
        #define RAPI
    #endif
#endif

#define CURRENT_FILE __FILE__
#define CURRENT_LINE __LINE__

#ifdef _MSC_VER
    #define CURRENT_FUNC __FUNCTION__
#else
    #define CURRENT_FUNC __func__
#endif

#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define CONCAT(a, b) a ## b
#define TOSTRING(x) #x

#define ARRAY_SIZE(array) (sizeof((array)) / sizeof((array)[0]))
#define FOREACH(item, array) \
    for (unsigned long long _i = 0; _i < ARRAY_SIZE((array)); _i++) \
        if (((item) = array[_i]) || 1)

static_assert(sizeof(unsigned char)      == 1, "Unsigned char expected to be 1 Byte!");
static_assert(sizeof(unsigned short)     == 2, "Unsigned short expected to be 1 Byte!");
static_assert(sizeof(unsigned int)       == 4, "Unsigned int expected to be 1 Byte!");
static_assert(sizeof(unsigned long long) == 8, "Unsigned long long expected to be 1 Byte!");

static_assert(sizeof(signed char)        == 1, "Signed char expected to be 1 Byte!");
static_assert(sizeof(signed short)       == 2, "Signed short expected to be 1 Byte!");
static_assert(sizeof(signed int)         == 4, "Signed int expected to be 1 Byte!");
static_assert(sizeof(signed long long)   == 8, "Signed long long expected to be 1 Byte!");

static_assert(sizeof(float)              == 4, "Float expected to be 1 Byte!");
static_assert(sizeof(double)             == 8, "Double expected to be 1 Byte!");

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;

typedef float               f32;
typedef double              f64;

typedef bool                b8;
typedef signed int          b32;

typedef unsigned long long  usize;
typedef signed long long    ssize;

typedef void *              phandle;
// typedef long long double    f128;




