/**
 * Support for 128-bit integers.
 */
#ifndef SYSLABCRT_DECL_COMPAT128BIT_HPP
#define SYSLABCRT_DECL_COMPAT128BIT_HPP
#if defined(__i386__)
#define NO_SUPPORT_INT128
#else
/* all GNU compilers */
#if (defined(__GNUC__) || defined(__clang__)) && defined(__SIZEOF_INT128__)

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wpragmas"
// #pragma GCC diagnostic ignored "-Wpedantic"
// #pragma GCC diagnostic ignored "-pedantic"
typedef unsigned __int128 uint128_t;
// #pragma GCC diagnostic pop

#define UINT128_C(n) ((uint128_t)n)
#define UINT128_MAX (~UINT128_C(0))

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wpragmas"
// #pragma GCC diagnostic ignored "-Wpedantic"
// #pragma GCC diagnostic ignored "-pedantic"
typedef __int128 int128_t;
// #pragma GCC diagnostic pop

#define INT128_C(n) ((int128_t)n)
#define INT128_MAX ((int128_t)(UINT128_MAX >> 1))
#define INT128_MIN (-INT128_MAX - 1)

// arm32
#elif (defined(__GNUC__) || defined(__clang__)) && !defined(__SIZEOF_INT128__)

// ptr_t内存对齐
#define PLATFORM_32

#include "syslabcrt-intrinsics/declarations/int128.hpp" // IWYU pragma: export

#define uint128_t uint128

#define UINT128_C(n) ((uint128_t)n)
#define UINT128_MAX (~UINT128_C(0))

#define int128_t int128

#define INT128_C(n) ((int128_t)n)
#define INT128_MAX ((int128_t)(UINT128_MAX >> 1))
#define INT128_MIN (-INT128_MAX - 1)

#else
#define NO_SUPPORT_INT128
#endif /* defined(__GNUC__) || defined(__clang__) && defined(__SIZEOF_INT128__) */

#endif
#endif