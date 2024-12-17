#ifndef SYSLABCRT_DEF_INTRIN_BITWISE_HPP
#define SYSLABCRT_DEF_INTRIN_BITWISE_HPP

#include "syslabcrt-intrinsics/intrinsics_core.hpp"
#include <cassert>
#include <cstdint>
#include <stdint.h>

namespace syslab
{
namespace rt
{
namespace intrinsics
{

#ifdef _MSC_VER
#include <intrin.h>

uint32_t __inline _sj_ctz(uint32_t value)
{
    unsigned long trailing_zero = 0;

    if (_BitScanForward(&trailing_zero, value))
    {
        return trailing_zero;
    }
    else
    {
        // This is undefined, I better choose 32 than 0
        return 32;
    }
}

uint32_t __inline _sj_clz(uint32_t value)
{
    unsigned long leading_zero = 0;

    if (_BitScanReverse(&leading_zero, value))
    {
        return 31 - leading_zero;
    }
    else
    {
        // Same remarks as above
        return 32;
    }
}

uint64_t __inline _sj_ctzll(uint64_t value)
{
    unsigned long index;
    if (_BitScanForward64(&index, value))
    {
        return (int)index;
    }
    else
    {
        // Undefined behavior for x == 0 in GCC's __builtin_ctzll.
        // Return a defined value like 64, or handle as needed.
        return 64;
    }
}

uint64_t __inline _sj_clzll(uint64_t x)
{
    unsigned long index;
    if (_BitScanReverse64(&index, x))
    {
        // Calculate the number of leading zeros
        return (int)(63 - index);
    }
    else
    {
        // Undefined behavior for x == 0 in GCC's __builtin_clzll.
        // Return 64 as a defined value or handle as needed.
        return 64;
    }
}
#define _sj_popcount __popcnt
#define _sj_popcountll __popcnt64

#else
#define _sj_ctz __builtin_ctz
#define _sj_clz __builtin_clz
#define _sj_ctzll __builtin_ctzll
#define _sj_clzll __builtin_clzll
#define _sj_popcount __builtin_popcount
#define _sj_popcountll __builtin_popcountll
#endif

CXX11_CONSTEXPR inline int8_t _sj_and_int_i8(int8_t x, int8_t y)
{
    return _sj_and_int(x, y);
}
CXX11_CONSTEXPR inline int16_t _sj_and_int_i16(int16_t x, int16_t y)
{
    return _sj_and_int(x, y);
}
CXX11_CONSTEXPR inline int32_t _sj_and_int_i32(int32_t x, int32_t y)
{
    return _sj_and_int(x, y);
}
CXX11_CONSTEXPR inline int64_t _sj_and_int_i64(int64_t x, int64_t y)
{
    return _sj_and_int(x, y);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_and_int_i128(int128_t x, int128_t y)
{
    auto v = x & y;
    return v;
}
#endif
CXX11_CONSTEXPR inline uint8_t _sj_and_int_u8(uint8_t x, uint8_t y)
{
    return _sj_and_int(x, y);
}
CXX11_CONSTEXPR inline uint16_t _sj_and_int_u16(uint16_t x, uint16_t y)
{
    return _sj_and_int(x, y);
}
CXX11_CONSTEXPR inline uint32_t _sj_and_int_u32(uint32_t x, uint32_t y)
{
    return _sj_and_int(x, y);
}
CXX11_CONSTEXPR inline uint64_t _sj_and_int_u64(uint64_t x, uint64_t y)
{
    return _sj_and_int(x, y);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_and_int_u128(uint128_t x, uint128_t y)
{
    auto v = x & y;
    return v;
}
#endif
CXX11_CONSTEXPR inline int8_t _sj_or_int_i8(int8_t x, int8_t y)
{
    return _sj_or_int(x, y);
}
CXX11_CONSTEXPR inline int16_t _sj_or_int_i16(int16_t x, int16_t y)
{
    return _sj_or_int(x, y);
}
CXX11_CONSTEXPR inline int32_t _sj_or_int_i32(int32_t x, int32_t y)
{
    return _sj_or_int(x, y);
}
CXX11_CONSTEXPR inline int64_t _sj_or_int_i64(int64_t x, int64_t y)
{
    return _sj_or_int(x, y);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_or_int_i128(int128_t x, int128_t y)
{
    auto v = x | y;
    return v;
}
#endif
CXX11_CONSTEXPR inline uint8_t _sj_or_int_u8(uint8_t x, uint8_t y)
{
    return _sj_or_int(x, y);
}
CXX11_CONSTEXPR inline uint16_t _sj_or_int_u16(uint16_t x, uint16_t y)
{
    return _sj_or_int(x, y);
}
CXX11_CONSTEXPR inline uint32_t _sj_or_int_u32(uint32_t x, uint32_t y)
{
    return _sj_or_int(x, y);
}
CXX11_CONSTEXPR inline uint64_t _sj_or_int_u64(uint64_t x, uint64_t y)
{
    return _sj_or_int(x, y);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_or_int_u128(uint128_t x, uint128_t y)
{
    auto v = x | y;
    return v;
}
#endif
CXX11_CONSTEXPR inline int8_t _sj_xor_int_i8(int8_t x, int8_t y)
{
    return _sj_xor_int(x, y);
}
CXX11_CONSTEXPR inline int16_t _sj_xor_int_i16(int16_t x, int16_t y)
{
    return _sj_xor_int(x, y);
}
CXX11_CONSTEXPR inline int32_t _sj_xor_int_i32(int32_t x, int32_t y)
{
    return _sj_xor_int(x, y);
}
CXX11_CONSTEXPR inline int64_t _sj_xor_int_i64(int64_t x, int64_t y)
{
    return _sj_xor_int(x, y);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_xor_int_i128(int128_t x, int128_t y)
{
    auto v = x ^ y;
    return v;
}
#endif
CXX11_CONSTEXPR inline uint8_t _sj_xor_int_u8(uint8_t x, uint8_t y)
{
    return _sj_xor_int(x, y);
}
CXX11_CONSTEXPR inline uint16_t _sj_xor_int_u16(uint16_t x, uint16_t y)
{
    return _sj_xor_int(x, y);
}
CXX11_CONSTEXPR inline uint32_t _sj_xor_int_u32(uint32_t x, uint32_t y)
{
    return _sj_xor_int(x, y);
}
CXX11_CONSTEXPR inline uint64_t _sj_xor_int_u64(uint64_t x, uint64_t y)
{
    return _sj_xor_int(x, y);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_xor_int_u128(uint128_t x, uint128_t y)
{
    auto v = x ^ y;
    return v;
}
#endif
CXX11_CONSTEXPR inline int8_t _sj_shl_int_i8(int8_t x, uint64_t shift)
{
    return _sj_shl_int(x, shift);
}
CXX11_CONSTEXPR inline int16_t _sj_shl_int_i16(int16_t x, uint64_t shift)
{
    return _sj_shl_int(x, shift);
}
CXX11_CONSTEXPR inline int32_t _sj_shl_int_i32(int32_t x, uint64_t shift)
{
    return _sj_shl_int(x, shift);
}
CXX11_CONSTEXPR inline int64_t _sj_shl_int_i64(int64_t x, uint64_t shift)
{
    return _sj_shl_int(x, shift);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_shl_int_i128(int128_t x, uint64_t shift)
{
    auto res = x << shift;
    return res;
}
#endif
CXX11_CONSTEXPR inline uint8_t _sj_shl_int_u8(uint8_t x, uint64_t shift)
{
    return _sj_shl_int(x, shift);
}
CXX11_CONSTEXPR inline uint16_t _sj_shl_int_u16(uint16_t x, uint64_t shift)
{
    return _sj_shl_int(x, shift);
}
CXX11_CONSTEXPR inline uint32_t _sj_shl_int_u32(uint32_t x, uint64_t shift)
{
    return _sj_shl_int(x, shift);
}
CXX11_CONSTEXPR inline uint64_t _sj_shl_int_u64(uint64_t x, uint64_t shift)
{
    return _sj_shl_int(x, shift);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_shl_int_u128(uint128_t x, uint64_t shift)
{
    int shift_limit = 128 * 8 - 1;
    if (shift > shift_limit)
    {
        return 0;
    }
    auto res = x << shift;
    return res;
}
#endif
CXX11_CONSTEXPR inline int8_t _sj_lshr_int_i8(int8_t x, uint64_t shift)
{
    return _sj_lshr_int(x, shift);
}
CXX11_CONSTEXPR inline int16_t _sj_lshr_int_i16(int16_t x, uint64_t shift)
{
    return _sj_lshr_int(x, shift);
}
CXX11_CONSTEXPR inline int32_t _sj_lshr_int_i32(int32_t x, uint64_t shift)
{
    return _sj_lshr_int(x, shift);
}
CXX11_CONSTEXPR inline int64_t _sj_lshr_int_i64(int64_t x, uint64_t shift)
{
    return _sj_lshr_int(x, shift);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_lshr_int_i128(int128_t x, uint64_t shift)
{
    uint128_t casted_x = static_cast<uint128_t>(x);
    int shift_limit = sizeof(int128_t) * 8 - 1;
    casted_x = shift > shift_limit ? 0 : (casted_x >> shift);

    return static_cast<int128_t>(casted_x);
}
#endif
CXX11_CONSTEXPR inline uint8_t _sj_lshr_int_u8(uint8_t x, uint64_t shift)
{
    return _sj_lshr_int(x, shift);
}
CXX11_CONSTEXPR inline uint16_t _sj_lshr_int_u16(uint16_t x, uint64_t shift)
{
    return _sj_lshr_int(x, shift);
}
CXX11_CONSTEXPR inline uint32_t _sj_lshr_int_u32(uint32_t x, uint64_t shift)
{
    return _sj_lshr_int(x, shift);
}
CXX11_CONSTEXPR inline uint64_t _sj_lshr_int_u64(uint64_t x, uint64_t shift)
{
    return _sj_lshr_int(x, shift);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_lshr_int_u128(uint128_t x, uint64_t shift)
{
    int shift_limit = sizeof(uint128_t) * 8 - 1;
    return shift > shift_limit ? 0 : x >> shift;
}
#endif
CXX11_CONSTEXPR inline int8_t _sj_ashr_int_i8(int8_t x, uint64_t shift)
{
    return _sj_ashr_int(x, shift);
}
CXX11_CONSTEXPR inline int16_t _sj_ashr_int_i16(int16_t x, uint64_t shift)
{
    return _sj_ashr_int(x, shift);
}
CXX11_CONSTEXPR inline int32_t _sj_ashr_int_i32(int32_t x, uint64_t shift)
{
    return _sj_ashr_int(x, shift);
}
CXX11_CONSTEXPR inline int64_t _sj_ashr_int_i64(int64_t x, uint64_t shift)
{
    return _sj_ashr_int(x, shift);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_ashr_int_i128(int128_t x, uint64_t shift)
{
    int shift_limit = sizeof(int128_t) * 8 - 1;
    return shift > shift_limit ? x >> shift_limit : x >> shift;
}
#endif
CXX11_CONSTEXPR inline uint8_t _sj_ashr_int_u8(uint8_t x, uint64_t shift)
{
    return _sj_ashr_int(x, shift);
}
CXX11_CONSTEXPR inline uint16_t _sj_ashr_int_u16(uint16_t x, uint64_t shift)
{
    return _sj_ashr_int(x, shift);
}
CXX11_CONSTEXPR inline uint32_t _sj_ashr_int_u32(uint32_t x, uint64_t shift)
{
    return _sj_ashr_int(x, shift);
}
CXX11_CONSTEXPR inline uint64_t _sj_ashr_int_u64(uint64_t x, uint64_t shift)
{
    return _sj_ashr_int(x, shift);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_ashr_int_u128(uint128_t x, uint64_t shift)
{
    int shift_limit = sizeof(int128_t) * 8 - 1;
    return shift > shift_limit ? x >> shift_limit : x >> shift;
}
#endif
CXX11_CONSTEXPR inline int8_t _sj_bswap_int_i8(int8_t x)
{
    return x;
}
CXX11_CONSTEXPR inline int16_t _sj_bswap_int_i16(int16_t x)
{
#if defined(_MSC_VER)
    uint16_t x_uint = (uint16_t)x;
    uint16_t swapped = (x_uint >> 8) | (x_uint << 8);
    return (int16_t)swapped;
#else
    return __builtin_bswap16(x);
#endif
}
CXX11_CONSTEXPR inline int32_t _sj_bswap_int_i32(int32_t x)
{
#if defined(_MSC_VER)
    uint32_t x_uint = (uint32_t)x;
    uint32_t swapped = (x_uint >> 16) | (x_uint << 16);
    return (int32_t)swapped;
#else
    return __builtin_bswap32(x);
#endif
}
CXX11_CONSTEXPR inline int64_t _sj_bswap_int_i64(int64_t x)
{
#if defined(_MSC_VER)
    uint64_t x_uint = (uint64_t)x;
    uint64_t swapped = (x_uint >> 32) | (x_uint << 32);
    return (int64_t)swapped;
#else
    return __builtin_bswap64(x);
#endif
}
CXX11_CONSTEXPR inline uint8_t _sj_bswap_int_u8(uint8_t x)
{
    return x;
}
CXX11_CONSTEXPR inline uint16_t _sj_bswap_int_u16(uint16_t x)
{
#if defined(_MSC_VER)
    return (x >> 8) | (x << 8);
#else
    return __builtin_bswap16(x);
#endif
}
CXX11_CONSTEXPR inline uint32_t _sj_bswap_int_u32(uint32_t x)
{
#if defined(_MSC_VER)
    return (x >> 16) | (x << 16);
#else
    return __builtin_bswap32(x);
#endif
}
CXX11_CONSTEXPR inline uint64_t _sj_bswap_int_u64(uint64_t x)
{
#if defined(_MSC_VER)
    return (x >> 32) | (x << 32);
#else
    return __builtin_bswap64(x);
#endif
}
CXX11_CONSTEXPR inline int8_t _sj_not_int_i8(int8_t x)
{
    return _sj_not_int(x);
}
CXX11_CONSTEXPR inline int16_t _sj_not_int_i16(int16_t x)
{
    return _sj_not_int(x);
}
CXX11_CONSTEXPR inline int32_t _sj_not_int_i32(int32_t x)
{
    return _sj_not_int(x);
}
CXX11_CONSTEXPR inline int64_t _sj_not_int_i64(int64_t x)
{
    return _sj_not_int(x);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_not_int_i128(int128_t x)
{
    return _sj_not_int(x);
}
#endif
CXX11_CONSTEXPR inline uint8_t _sj_not_int_u8(uint8_t x)
{
    return _sj_not_int(x);
}
CXX11_CONSTEXPR inline uint16_t _sj_not_int_u16(uint16_t x)
{
    return _sj_not_int(x);
}
CXX11_CONSTEXPR inline uint32_t _sj_not_int_u32(uint32_t x)
{
    return _sj_not_int(x);
}
CXX11_CONSTEXPR inline uint64_t _sj_not_int_u64(uint64_t x)
{
    return _sj_not_int(x);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_not_int_u128(uint128_t x)
{
    return _sj_not_int(x);
}
#endif
CXX11_CONSTEXPR inline uint8_t _sj_ctlz_bool(uint8_t x)
{
    return x;
}
CXX11_CONSTEXPR inline uint8_t _sj_ctlz_int(uint8_t u)
{
    static_assert(sizeof(unsigned int) >= sizeof(uint8_t), "_sj_ctlz_int: sizeof(unsigned int) >= sizeof(uint8_t)");
    CXX11_CONSTEXPR auto diff = 8 * (sizeof(unsigned int) - sizeof(uint8_t));
    auto ret = _sj_clz(u) - diff;
    return static_cast<uint8_t>(ret);
}
CXX11_CONSTEXPR inline uint16_t _sj_ctlz_int(uint16_t u)
{
    static_assert(sizeof(unsigned int) >= sizeof(uint16_t), "_sj_ctlz_int: sizeof(unsigned int) >= sizeof(uint16_t)");
    CXX11_CONSTEXPR auto diff = 8 * (sizeof(unsigned int) - sizeof(uint16_t));
    auto ret = _sj_clz(u) - diff;
    return static_cast<uint16_t>(ret);
}
CXX11_CONSTEXPR inline uint32_t _sj_ctlz_int(uint32_t u)
{
    static_assert(sizeof(unsigned int) == sizeof(uint32_t), "_sj_ctlz_int: sizeof(unsigned int) == sizeof(uint32_t)");
    auto ret = _sj_clz(u);
    return static_cast<uint32_t>(ret);
}
CXX11_CONSTEXPR inline uint64_t _sj_ctlz_int(uint64_t u)
{
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t),
                  "_sj_ctlz_int: sizeof(unsigned long long) == sizeof(uint64_t)");
    auto ret = _sj_clzll(u);
    return static_cast<uint64_t>(ret);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_ctlz_int(uint128_t u)
{
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t),
                  "_sj_ctlz_int: sizeof(unsigned long long) == sizeof(uint64_t)");
    uint64_t hi = 0, lo = 0;
    int b = 128;

    if ((hi = (uint64_t)(u >> 64)) != 0)
    {
        b = _sj_clzll(hi);
    }
    else if ((lo = (uint64_t)(u & ~0ULL)) != 0)
    {
        b = _sj_clzll(lo) + 64;
    }
    return b;
}
#endif
CXX11_CONSTEXPR inline int8_t _sj_ctlz_int(int8_t x)
{
    uint8_t u = static_cast<uint8_t>(x);
    return _sj_ctlz_int(u);
}
CXX11_CONSTEXPR inline int16_t _sj_ctlz_int(int16_t x)
{
    uint16_t u = static_cast<uint16_t>(x);
    return _sj_ctlz_int(u);
}
CXX11_CONSTEXPR inline int32_t _sj_ctlz_int(int32_t x)
{
    uint32_t u = static_cast<uint32_t>(x);
    return _sj_ctlz_int(u);
}
CXX11_CONSTEXPR inline int64_t _sj_ctlz_int(int64_t x)
{
    uint64_t u = static_cast<uint64_t>(x);
    return _sj_ctlz_int(u);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_ctlz_int(int128_t x)
{
    uint128_t u = static_cast<uint128_t>(x);
    return (int128_t)_sj_ctlz_int(u);
}
#endif

CXX11_CONSTEXPR inline uint8_t _sj_cttz_bool(uint8_t x)
{
    return 0;
}

CXX11_CONSTEXPR inline uint8_t _sj_cttz_int(uint8_t u)
{
    static_assert(sizeof(unsigned int) >= sizeof(int8_t), "_sj_cttz_int: sizeof(unsigned int) >= sizeof(int8_t)");
    CXX11_CONSTEXPR uint8_t _Max = static_cast<uint8_t>(-1);
    auto ret = _sj_ctz(static_cast<unsigned int>(~_Max | u));
    return static_cast<uint8_t>(ret);
}
CXX11_CONSTEXPR inline uint16_t _sj_cttz_int(uint16_t u)
{
    static_assert(sizeof(unsigned int) >= sizeof(int16_t), "_sj_cttz_int: sizeof(unsigned int) >= sizeof(int16_t)");
    CXX11_CONSTEXPR uint16_t _Max = static_cast<uint16_t>(-1);
    auto ret = _sj_ctz(static_cast<unsigned int>(~_Max | u));
    return static_cast<uint16_t>(ret);
}
CXX11_CONSTEXPR inline uint32_t _sj_cttz_int(uint32_t u)
{
    static_assert(sizeof(unsigned int) >= sizeof(int32_t), "_sj_cttz_int: sizeof(unsigned int) >= sizeof(int32_t)");
    CXX11_CONSTEXPR uint32_t _Max = static_cast<uint32_t>(-1);
    auto ret = _sj_ctz(static_cast<unsigned int>(~_Max | u));
    return static_cast<uint32_t>(ret);
}
CXX11_CONSTEXPR inline uint64_t _sj_cttz_int(uint64_t x)
{
    static_assert(sizeof(unsigned long long) == sizeof(int64_t),
                  "_sj_cttz_int: sizeof(unsigned long long) == sizeof(int64_t)");
    uint64_t u = static_cast<uint64_t>(x);
    auto ret = _sj_ctzll(u);
    return static_cast<uint64_t>(ret);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_cttz_int(uint128_t u)
{
    static_assert(sizeof(unsigned long long) == sizeof(int64_t),
                  "_sj_cttz_int: sizeof(unsigned long long) == sizeof(int64_t)");
    uint64_t hi = 0, lo = 0;
    int b = 128;

    if ((lo = (uint64_t)(u & ~0ULL)) != 0)
    {
        b = _sj_ctzll(lo);
    }
    else if ((hi = (uint64_t)(u >> 64)) != 0)
    {
        b = _sj_ctzll(hi) + 64;
    }
    return b;
}
#endif
CXX11_CONSTEXPR inline int8_t _sj_cttz_int(int8_t x)
{
    uint8_t u = static_cast<uint8_t>(x);
    return _sj_cttz_int(u);
}
CXX11_CONSTEXPR inline int16_t _sj_cttz_int(int16_t x)
{
    uint16_t u = static_cast<uint16_t>(x);
    return _sj_cttz_int(u);
}
CXX11_CONSTEXPR inline int32_t _sj_cttz_int(int32_t x)
{
    uint32_t u = static_cast<uint32_t>(x);
    return _sj_cttz_int(u);
}
CXX11_CONSTEXPR inline int64_t _sj_cttz_int(int64_t x)
{
    uint64_t u = static_cast<uint64_t>(x);
    return _sj_cttz_int(u);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_cttz_int(int128_t x)
{
    uint128_t u = static_cast<uint128_t>(x);
    return (int128_t)_sj_cttz_int(u);
}
#endif
CXX11_CONSTEXPR inline uint8_t _sj_ctpop_bool(uint8_t x)
{
    return x;
}
CXX11_CONSTEXPR inline uint8_t _sj_ctpop_int(uint8_t x)
{
    static_assert(sizeof(unsigned int) >= sizeof(uint8_t), "_sj_ctpop_int: sizeof(unsigned int) >= sizeof(uint8_t)");
    return _sj_popcount(static_cast<uint32_t>(x));
}
CXX11_CONSTEXPR inline uint16_t _sj_ctpop_int(uint16_t x)
{
    static_assert(sizeof(unsigned int) >= sizeof(uint16_t), "_sj_ctpop_int: sizeof(unsigned int) >= sizeof(uint16_t)");
    return _sj_popcount(static_cast<uint16_t>(x));
}
CXX11_CONSTEXPR inline uint32_t _sj_ctpop_int(uint32_t x)
{
    static_assert(sizeof(unsigned int) >= sizeof(uint32_t), "_sj_ctpop_int: sizeof(unsigned int) >= sizeof(uint32_t)");
    return _sj_popcount(static_cast<uint32_t>(x));
}
CXX11_CONSTEXPR inline uint64_t _sj_ctpop_int(uint64_t x)
{
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t),
                  "_sj_ctpop_int: sizeof(unsigned long long) == sizeof(uint64_t)");
    return _sj_popcountll(static_cast<uint64_t>(x));
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline uint128_t _sj_ctpop_int(uint128_t x)
{
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t),
                  "_sj_ctpop_int: sizeof(unsigned long long) == sizeof(uint64_t)");
    uint128_t u = static_cast<uint128_t>(x);
    uint64_t hi = 0, lo = 0;

    hi = static_cast<uint64_t>(u >> 64);
    lo = static_cast<uint64_t>(u & ~0ULL);
    int b = _sj_popcountll(hi) + _sj_popcountll(lo);
    return b;
}
#endif
CXX11_CONSTEXPR inline int8_t _sj_ctpop_int(int8_t x)
{
    uint8_t u = static_cast<uint8_t>(x);
    return _sj_ctpop_int(u);
}
CXX11_CONSTEXPR inline int16_t _sj_ctpop_int(int16_t x)
{
    uint16_t u = static_cast<uint16_t>(x);
    return _sj_ctpop_int(u);
}
CXX11_CONSTEXPR inline int32_t _sj_ctpop_int(int32_t x)
{
    uint32_t u = static_cast<uint32_t>(x);
    return _sj_ctpop_int(u);
}
CXX11_CONSTEXPR inline int64_t _sj_ctpop_int(int64_t x)
{
    uint64_t u = static_cast<uint64_t>(x);
    return _sj_ctpop_int(u);
}
#if !defined(NO_SUPPORT_INT128)
CXX11_CONSTEXPR inline int128_t _sj_ctpop_int(int128_t x)
{
    uint128_t u = static_cast<uint128_t>(x);
    return (int128_t)_sj_ctpop_int(u);
}
#endif

} // namespace intrinsics
} // namespace rt
} // namespace syslab

#endif // SYSLABCRT_DEF_INTRIN_BITWISE_HPP