#pragma once
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// OS
#if defined(WIN64) || defined(_WIN64)
    #define OS_WIN64
    #define OS_WIN32
#elif defined(WIN32)|| defined(_WIN32)
    #define OS_WIN32
#elif defined(ANDROID) || defined(__ANDROID__)
    #define OS_ANDROID
    #define OS_LINUX
#elif defined(linux) || defined(__linux) || defined(__linux__)
    #define OS_LINUX
#elif defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
    #include <TargetConditionals.h>
    #if defined(TARGET_OS_MAC) && TARGET_OS_MAC
        #define OS_MAC
    #elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
        #define OS_IOS
    #endif
    #define OS_DARWIN
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
    #define OS_FREEBSD
    #define OS_BSD
#elif defined(__NetBSD__)
    #define OS_NETBSD
    #define OS_BSD
#elif defined(__OpenBSD__)
    #define OS_OPENBSD
    #define OS_BSD
#elif defined(sun) || defined(__sun) || defined(__sun__)
    #define OS_SOLARIS
#else
    #warning "Untested operating system platform!"
#endif

#if defined(OS_WIN32) || defined(OS_WIN64)
    #undef  OS_UNIX
    #define OS_WIN
#else
    #define OS_UNIX
#endif

// ARCH
#if defined(__i386) || defined(__i386__) || defined(_M_IX86)
    #define ARCH_X86
    #define ARCH_X86_32
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
    #define ARCH_X64
    #define ARCH_X86_64
#elif defined(__arm__)
    #define ARCH_ARM
#elif defined(__aarch64__) || defined(__ARM64__)
    #define ARCH_ARM64
#elif defined(__mips__)
    #define ARCH_MIPS
#elif defined(__mips64__)
    #define ARCH_MIPS64
#else
    #warning "Untested hardware architecture!"
#endif

/// 数值对齐
#define NUM_ALIGN(d, a) (((d) + (a - 1)) & ~(a - 1))
/// 指针对齐
#define PTR_ALIGN(p, a) (uint8_t *)(((uintptr_t)(p) + ((uintptr_t)a - 1)) & ~((uintptr_t)a - 1))

#define BITMAP_DEF(name, n, val) uint8_t name[NUM_ALIGN((n), CHAR_BIT)/CHAR_BIT] = {(val) ? UINT8_MAX : 0}
#define BITMAP_SET(p, n) ((p)[(n) >> 3] |= (0x01 << ((n) & (CHAR_BIT - 1))))
#define BITMAP_CLR(p, n) ((p)[(n) >> 3] &= ~(0x01 << ((n) & (CHAR_BIT - 1))))
#define BITMAP_GET(p, n) (((p)[(n) >> 3] & (0x01 << ((n) & (CHAR_BIT - 1)))) ? 1 : 0)

#ifndef ABS
#define ABS(n)  ((n) > 0 ? (n) : -(n))
#endif

#ifndef NABS
#define NABS(n) ((n) < 0 ? (n) : -(n))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))
#endif

#ifndef BITSET
#define BITSET(p, n) (*(p) |= (1u << (n)))
#endif

#ifndef BITCLR
#define BITCLR(p, n) (*(p) &= ~(1u << (n)))
#endif

#ifndef BITGET
#define BITGET(i, n) ((i) & (1u << (n)))
#endif

/*
#ifndef CR
#define CR      '\r'
#endif

#ifndef LF
#define LF      '\n'
#endif

#ifndef CRLF
#define CRLF    "\r\n"
#endif
*/

#define FLOAT_PRECISION     1e-6
#define FLOAT_EQUAL_ZERO(f) (ABS(f) < FLOAT_PRECISION)

#ifndef INFINITE
#define INFINITE    (uint32_t)-1
#endif

/*
ASCII:
[0, 0x20)    control-charaters
[0x20, 0x7F) printable-charaters

0x0A => LF
0x0D => CR
0x20 => SPACE
0x7F => DEL

[0x09, 0x0D] => \t\n\v\f\r
[0x30, 0x39] => 0~9
[0x41, 0x5A] => A~Z
[0x61, 0x7A] => a~z
*/

#ifndef IS_ALPHA
#define IS_ALPHA(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#endif

// NOTE: IS_NUM conflicts with mysql.h
#ifndef IS_DIGIT
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#endif

#ifndef IS_ALPHANUM
#define IS_ALPHANUM(c) (IS_ALPHA(c) || IS_DIGIT(c))
#endif

#ifndef IS_CNTRL
#define IS_CNTRL(c) ((c) >= 0 && (c) < 0x20)
#endif

#ifndef IS_GRAPH
#define IS_GRAPH(c) ((c) >= 0x20 && (c) < 0x7F)
#endif

#ifndef IS_HEX
#define IS_HEX(c) (IS_DIGIT(c) || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
#endif

#ifndef IS_LOWER
#define IS_LOWER(c) (((c) >= 'a' && (c) <= 'z'))
#endif

#ifndef IS_UPPER
#define IS_UPPER(c) (((c) >= 'A' && (c) <= 'Z'))
#endif

#ifndef LOWER
#define LOWER(c)    ((c) | 0x20)
#endif

#ifndef UPPER
#define UPPER(c)    ((c) & ~0x20)
#endif

// LD, LU, LLD, LLU for explicit conversion of integer
// #ifndef LD
// #define LD(v)   ((long)(v))
// #endif

// #ifndef LU
// #define LU(v)   ((unsigned long)(v))
// #endif

#ifndef LLD
#define LLD(v)  ((long long)(v))
#endif

#ifndef LLU
#define LLU(v)  ((unsigned long long)(v))
#endif

// MAKEWORD, HIBYTE, LOBYTE
#ifndef MAKEWORD
#define MAKEWORD(h, l)  ( (((WORD)h) << 8) | (l & 0xff) )
#endif

#ifndef HIBYTE
#define HIBYTE(w) ( (BYTE)(((WORD)w) >> 8) )
#endif

#ifndef LOBYTE
#define LOBYTE(w) ( (BYTE)(w & 0xff) )
#endif

// MAKELONG, HIWORD, LOWORD
#ifndef MAKELONG
#define MAKELONG(h, l)   ( ((int32_t)h) << 16 | (l & 0xffff) )
#endif

#ifndef HIWORD
#define HIWORD(n)        ( (WORD)(((int32_t)n) >> 16) )
#endif

#ifndef LOWORD
#define LOWORD(n)        ( (WORD)(n & 0xffff) )
#endif

// MAKEINT64, HIINT, LOINT
#ifndef MAKEINT64
#define MAKEINT64(h, l)   ( ((int64_t)h) << 32 | (l & 0xffffffff) )
#endif

#ifndef HIINT
#define HIINT(n)        ( (int32_t)(((int64_t)n) >> 32) )
#endif

#ifndef LOINT
#define LOINT(n)        ( (int32_t)(n & 0xffffffff) )
#endif

#ifndef MAKE_FOURCC
#define MAKE_FOURCC(a, b, c, d) \
( ((uint32)d) | ( ((uint32)c) << 8 ) | ( ((uint32)b) << 16 ) | ( ((uint32)a) << 24 ) )
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef LIMIT
#define LIMIT(lower, v, upper) ((v) < (lower) ? (lower) : (v) > (upper) ? (upper) : (v))
#endif

#ifndef MAX_PATH
#define MAX_PATH    260
#endif

#ifndef SAFE_ALLOC
#define SAFE_ALLOC(p, size)\
    do {\
        void* ptr = ::malloc(size);\
        if (!ptr) {\
            ::fprintf(stderr, "malloc failed!\n");\
            ::exit(-1);\
        }\
        ::memset(ptr, 0, size);\
        *(void**)&(p) = ptr;\
    } while(0)
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p)    do {if (p) {::free(p); (p) = NULL;}} while(0)
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  do {if (p) {delete (p); (p) = NULL;}} while(0)
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) do {if (p) {delete[] (p); (p) = NULL;}} while(0)
#endif

#ifndef SAFE_CLOSE
#define SAFE_CLOSE(fd)  do {if ((fd) >= 0) {::close(fd); (fd) = -1;}} while(0)
#endif

#define STRINGIFY(x)    STRINGIFY_HELPER(x)
#define STRINGIFY_HELPER(x)     #x

#define STRINGCAT(x, y)  STRINGCAT_HELPER(x, y)
#define STRINGCAT_HELPER(x, y)  x##y

#ifndef offsetof
#define offsetof(type, member) \
((size_t)(&((type*)0)->member))
#endif

#ifndef offsetofend
#define offsetofend(type, member) \
(offsetof(type, member) + sizeof(((type*)0)->member))
#endif

#ifndef container_of
#define container_of(ptr, type, member) \
((type*)((char*)(ptr) - offsetof(type, member)))
#endif

/// 判断优化，都是判断为真的情况
#if (defined(__GNUC__) && __GNUC__ >= 3) || defined(__clang__)
#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif
#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif
#else
#ifndef unlikely
#define unlikely(x) (x)
#endif
#ifndef likely
#define likely(x) (x)
#endif
#endif
