#ifndef SYSLABCRT_DEF_INTRIN_INT128_HPP
#define SYSLABCRT_DEF_INTRIN_INT128_HPP

#ifdef SYSLABCC_HAVE_INTRINSIC_INT128
#error "SYSLABCC_HAVE_INTRINSIC_INT128 cannot be directly set"
#endif
#if (defined(__GNUC__) || defined(__clang__)) && defined(__SIZEOF_INT128__)
#define SYSLABCC_HAVE_INTRINSIC_INT128 1
#endif // (defined(__GNUC__) || defined(__clang__)) && defined(__SIZEOF_INT128__)


// ==============start here===========================================================================
#if !defined(SYSLABCC_HAVE_INTRINSIC_INT128)

// SYSLABCC_INTERNAL_RESTORE_DEPRECATED_DECLARATION_WARNING
#if defined(__GNUC__) || defined(__clang__)
// Clang also supports these GCC pragmas.
#define SYSLABCC_INTERNAL_DISABLE_DEPRECATED_DECLARATION_WARNING                                                            \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#define SYSLABCC_INTERNAL_RESTORE_DEPRECATED_DECLARATION_WARNING _Pragma("GCC diagnostic pop")
#elif defined(_MSC_VER)
#define SYSLABCC_INTERNAL_DISABLE_DEPRECATED_DECLARATION_WARNING _Pragma("warning(push)") _Pragma("warning(disable: 4996)")
#define SYSLABCC_INTERNAL_RESTORE_DEPRECATED_DECLARATION_WARNING _Pragma("warning(pop)")
#else
#define SYSLABCC_INTERNAL_DISABLE_DEPRECATED_DECLARATION_WARNING
#define SYSLABCC_INTERNAL_RESTORE_DEPRECATED_DECLARATION_WARNING
#endif // defined(__GNUC__) || defined(__clang__)

// SYSLABCC_IS_LITTLE_ENDIAN
// SYSLABCC_IS_BIG_ENDIAN
//
// Checks the endianness of the platform.
//
// Notes: uses the built in endian macros provided by GCC (since 4.6) and
// Clang (since 3.2); see
// https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html.
// Otherwise, if _WIN32, assume little endian. Otherwise, bail with an error.
#if defined(SYSLABCC_IS_BIG_ENDIAN)
#error "SYSLABCC_IS_BIG_ENDIAN cannot be directly set."
#endif
#if defined(SYSLABCC_IS_LITTLE_ENDIAN)
#error "SYSLABCC_IS_LITTLE_ENDIAN cannot be directly set."
#endif

#if (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define SYSLABCC_IS_LITTLE_ENDIAN 1
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define SYSLABCC_IS_BIG_ENDIAN 1
#elif defined(_WIN32)
#define SYSLABCC_IS_LITTLE_ENDIAN 1
#else
#error "SYSLABCC endian detection needs to be set up for your compiler"
#endif

#if defined(_MSC_VER)
// In very old versions of MSVC and when the /Zc:wchar_t flag is off, wchar_t is
// a typedef for unsigned short.  Otherwise wchar_t is mapped to the __wchar_t
// builtin type.  We need to make sure not to define operator wchar_t()
// alongside operator unsigned short() in these instances.
#define SYSLABCC_INTERNAL_WCHAR_T __wchar_t
#else // defined(_MSC_VER)
#define SYSLABCC_INTERNAL_WCHAR_T wchar_t
#endif // defined(_MSC_VER)

#include <bit>
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <math.h>
#include <sstream>
#include <string>

namespace syslab
{
namespace rt
{
namespace intrinsics
{

class int128; // forward declaration

class uint128
{
  public:
    uint128() = default;
    // Constructors from arithmetic types
    constexpr uint128(int v);                // NOLINT(runtime/explicit)
    constexpr uint128(unsigned int v);       // NOLINT(runtime/explicit)
    constexpr uint128(long v);               // NOLINT(runtime/int)
    constexpr uint128(unsigned long v);      // NOLINT(runtime/int)
    constexpr uint128(long long v);          // NOLINT(runtime/int)
    constexpr uint128(unsigned long long v); // NOLINT(runtime/int)
    constexpr uint128(int128 v);             // NOLINT(runtime/explicit)
    explicit uint128(float v);
    explicit uint128(double v);
    explicit uint128(long double v);

    // Assignment operators from arithmetic types
    uint128 &operator=(int v);
    uint128 &operator=(unsigned int v);
    uint128 &operator=(long v);               // NOLINT(runtime/int)
    uint128 &operator=(unsigned long v);      // NOLINT(runtime/int)
    uint128 &operator=(long long v);          // NOLINT(runtime/int)
    uint128 &operator=(unsigned long long v); // NOLINT(runtime/int)
    uint128 &operator=(int128 v);

    // Conversion operators to other arithmetic types
    constexpr explicit operator bool() const;
    constexpr explicit operator char() const;
    constexpr explicit operator signed char() const;
    constexpr explicit operator unsigned char() const;
    constexpr explicit operator char16_t() const;
    constexpr explicit operator char32_t() const;
    constexpr explicit operator SYSLABCC_INTERNAL_WCHAR_T() const;
    constexpr explicit operator short() const; // NOLINT(runtime/int)
    // NOLINTNEXTLINE(runtime/int)
    constexpr explicit operator unsigned short() const;
    constexpr explicit operator int() const;
    constexpr explicit operator unsigned int() const;
    constexpr explicit operator long() const; // NOLINT(runtime/int)
    // NOLINTNEXTLINE(runtime/int)
    constexpr explicit operator unsigned long() const;
    // NOLINTNEXTLINE(runtime/int)
    constexpr explicit operator long long() const;
    // NOLINTNEXTLINE(runtime/int)
    constexpr explicit operator unsigned long long() const;
    explicit operator float() const;
    explicit operator double() const;
    explicit operator long double() const;

    // Arithmetic operators.
    uint128 &operator+=(uint128 other);
    uint128 &operator-=(uint128 other);
    uint128 &operator*=(uint128 other);
    // Long division/modulo for uint128.
    uint128 &operator/=(uint128 other);
    uint128 &operator%=(uint128 other);
    uint128 operator++(int);
    uint128 operator--(int);
    uint128 &operator<<=(int);
    uint128 &operator>>=(int);
    uint128 &operator&=(uint128 other);
    uint128 &operator|=(uint128 other);
    uint128 &operator^=(uint128 other);
    uint128 &operator++();
    uint128 &operator--();

    // Returns the lower 64-bit value of a `uint128` value.
    friend constexpr uint64_t Uint128Low64(uint128 v);
    // Returns the higher 64-bit value of a `uint128` value.
    friend constexpr uint64_t Uint128High64(uint128 v);

    // MakeUInt128()
    //
    // Constructs a `uint128` numeric value from two 64-bit unsigned integers.
    // Note that this factory function is the only way to construct a `uint128`
    // from integer values greater than 2^64.
    //
    // Example:
    //
    //   SYSLABCC::uint128 big = SYSLABCC::MakeUint128(1, 0);
    friend constexpr uint128 MakeUint128(uint64_t high, uint64_t low);

    // Uint128Max()
    //
    // Returns the highest value for a 128-bit unsigned integer.
    friend constexpr uint128 Uint128Max();

  private:
    constexpr uint128(uint64_t high, uint64_t low);
    std::string ToString() const;

#if defined(SYSLABCC_IS_LITTLE_ENDIAN)
    uint64_t lo_;
    uint64_t hi_;
#elif defined(SYSLABCC_IS_BIG_ENDIAN)
    uint64_t hi_;
    uint64_t lo_;
#else
#error "Unsupported byte order: must be little-endian or big-endian."
#endif
};

// allow uint128 to be logged
std::ostream &operator<<(std::ostream &os, uint128 v);

constexpr uint128 Uint128Max()
{
    return uint128((std::numeric_limits<uint64_t>::max)(), (std::numeric_limits<uint64_t>::max)());
}

} // namespace intrinsics
} // namespace rt
} // namespace syslab

// Specialized numeric_limits for uint128.
namespace std
{
using syslab::rt::intrinsics::uint128;
using syslab::rt::intrinsics::Uint128Max;
template <> class numeric_limits<uint128>
{
  public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    SYSLABCC_INTERNAL_DISABLE_DEPRECATED_DECLARATION_WARNING
    static constexpr float_denorm_style has_denorm = denorm_absent;
    SYSLABCC_INTERNAL_RESTORE_DEPRECATED_DECLARATION_WARNING
    static constexpr bool has_denorm_loss = false;
    static constexpr float_round_style round_style = round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;
    static constexpr int digits = 128;
    static constexpr int digits10 = 38;
    static constexpr int max_digits10 = 0;
    static constexpr int radix = 2;
    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;

    static constexpr bool traps = numeric_limits<uint64_t>::traps;

    static constexpr bool tinyness_before = false;

    static constexpr uint128(min)()
    {
        return 0;
    }
    static constexpr uint128 lowest()
    {
        return 0;
    }
    static constexpr uint128(max)()
    {
        return Uint128Max();
    }
    static constexpr uint128 epsilon()
    {
        return 0;
    }
    static constexpr uint128 round_error()
    {
        return 0;
    }
    static constexpr uint128 infinity()
    {
        return 0;
    }
    static constexpr uint128 quiet_NaN()
    {
        return 0;
    }
    static constexpr uint128 signaling_NaN()
    {
        return 0;
    }
    static constexpr uint128 denorm_min()
    {
        return 0;
    }
};

} // namespace std

namespace syslab
{
namespace rt
{
namespace intrinsics
{
class int128
{
  public:
    int128() = default;

    // Constructors from arithmetic types
    constexpr int128(int v);                // NOLINT(runtime/explicit)
    constexpr int128(unsigned int v);       // NOLINT(runtime/explicit)
    constexpr int128(long v);               // NOLINT(runtime/int)
    constexpr int128(unsigned long v);      // NOLINT(runtime/int)
    constexpr int128(long long v);          // NOLINT(runtime/int)
    constexpr int128(unsigned long long v); // NOLINT(runtime/int)
    constexpr explicit int128(uint128 v);
    explicit int128(float v);
    explicit int128(double v);
    explicit int128(long double v);

    // Assignment operators from arithmetic types
    int128 &operator=(int v);
    int128 &operator=(unsigned int v);
    int128 &operator=(long v);               // NOLINT(runtime/int)
    int128 &operator=(unsigned long v);      // NOLINT(runtime/int)
    int128 &operator=(long long v);          // NOLINT(runtime/int)
    int128 &operator=(unsigned long long v); // NOLINT(runtime/int)

    // Conversion operators to other arithmetic types
    constexpr explicit operator bool() const;
    constexpr explicit operator char() const;
    constexpr explicit operator signed char() const;
    constexpr explicit operator unsigned char() const;
    constexpr explicit operator char16_t() const;
    constexpr explicit operator char32_t() const;
    constexpr explicit operator SYSLABCC_INTERNAL_WCHAR_T() const;
    constexpr explicit operator short() const; // NOLINT(runtime/int)
    // NOLINTNEXTLINE(runtime/int)
    constexpr explicit operator unsigned short() const;
    constexpr explicit operator int() const;
    constexpr explicit operator unsigned int() const;
    constexpr explicit operator long() const; // NOLINT(runtime/int)
    // NOLINTNEXTLINE(runtime/int)
    constexpr explicit operator unsigned long() const;
    // NOLINTNEXTLINE(runtime/int)
    constexpr explicit operator long long() const;
    // NOLINTNEXTLINE(runtime/int)
    constexpr explicit operator unsigned long long() const;
    explicit operator float() const;
    explicit operator double() const;
    explicit operator long double() const;

    // Trivial copy constructor, assignment operator and destructor.

    // Arithmetic operators
    int128 &operator+=(int128 other);
    int128 &operator-=(int128 other);
    int128 &operator*=(int128 other);
    int128 &operator/=(int128 other);
    int128 &operator%=(int128 other);
    int128 operator++(int); // postfix increment: i++
    int128 operator--(int); // postfix decrement: i--
    int128 &operator++();   // prefix increment:  ++i
    int128 &operator--();   // prefix decrement:  --i
    int128 &operator&=(int128 other);
    int128 &operator|=(int128 other);
    int128 &operator^=(int128 other);
    int128 &operator<<=(int amount);
    int128 &operator>>=(int amount);

    // Int128Low64()
    //
    // Returns the lower 64-bit value of a `int128` value.
    friend constexpr uint64_t Int128Low64(int128 v);

    // Int128High64()
    //
    // Returns the higher 64-bit value of a `int128` value.
    friend constexpr int64_t Int128High64(int128 v);

    // MakeInt128()
    //
    // Constructs a `int128` numeric value from two 64-bit integers. Note that
    // signedness is conveyed in the upper `high` value.
    //
    //   (int128(1) << 64) * high + low
    //
    // Note that this factory function is the only way to construct a `int128`
    // from integer values greater than 2^64 or less than -2^64.
    //
    // Example:
    //
    //   int128 big = MakeInt128(1, 0);
    //   int128 big_n = MakeInt128(-1, 0);
    friend constexpr int128 MakeInt128(int64_t high, uint64_t low);

    // Int128Max()
    //
    // Returns the maximum value for a 128-bit signed integer.
    friend constexpr int128 Int128Max();

    // Int128Min()
    //
    // Returns the minimum value for a 128-bit signed integer.
    friend constexpr int128 Int128Min();

  private:
    constexpr int128(int64_t high, uint64_t low);

    std::string ToString() const;

#if defined(SYSLABCC_IS_LITTLE_ENDIAN)
    uint64_t lo_;
    int64_t hi_;
#elif defined(SYSLABCC_IS_BIG_ENDIAN)
    int64_t hi_;
    uint64_t lo_;
#else // byte order
#error "Unsupported byte order: must be little-endian or big-endian."
#endif // byte order
};

std::ostream &operator<<(std::ostream &os, int128 v);

constexpr int128 Int128Max()
{
    return int128((std::numeric_limits<int64_t>::max)(), (std::numeric_limits<uint64_t>::max)());
}

constexpr int128 Int128Min()
{
    return int128((std::numeric_limits<int64_t>::min)(), 0);
}

} // namespace intrinsics
} // namespace rt
} // namespace syslab

// Specialized numeric_limits for int128.
namespace std
{
using syslab::rt::intrinsics::int128;
using syslab::rt::intrinsics::Int128Max;
using syslab::rt::intrinsics::Int128Min;
template <> class numeric_limits<int128>
{
  public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    SYSLABCC_INTERNAL_DISABLE_DEPRECATED_DECLARATION_WARNING
    static constexpr float_denorm_style has_denorm = denorm_absent;
    SYSLABCC_INTERNAL_RESTORE_DEPRECATED_DECLARATION_WARNING
    static constexpr bool has_denorm_loss = false;
    static constexpr float_round_style round_style = round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = false;
    static constexpr int digits = 127;
    static constexpr int digits10 = 38;
    static constexpr int max_digits10 = 0;
    static constexpr int radix = 2;
    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;
    static constexpr bool traps = numeric_limits<uint64_t>::traps;
    static constexpr bool tinyness_before = false;

    static constexpr int128(min)()
    {
        return Int128Min();
    }
    static constexpr int128 lowest()
    {
        return Int128Min();
    }
    static constexpr int128(max)()
    {
        return Int128Max();
    }
    static constexpr int128 epsilon()
    {
        return 0;
    }
    static constexpr int128 round_error()
    {
        return 0;
    }
    static constexpr int128 infinity()
    {
        return 0;
    }
    static constexpr int128 quiet_NaN()
    {
        return 0;
    }
    static constexpr int128 signaling_NaN()
    {
        return 0;
    }
    static constexpr int128 denorm_min()
    {
        return 0;
    }
};
} // namespace std

// =========Implementation details follow ===============
namespace syslab
{
namespace rt
{
namespace intrinsics
{

constexpr uint128 MakeUint128(uint64_t high, uint64_t low)
{
    return uint128(high, low);
}

// Assignment from integer types.

inline uint128 &uint128::operator=(int v)
{
    return *this = uint128(v);
}

inline uint128 &uint128::operator=(unsigned int v)
{
    return *this = uint128(v);
}

inline uint128 &uint128::operator=(long v)
{ // NOLINT(runtime/int)
    return *this = uint128(v);
}

// NOLINTNEXTLINE(runtime/int)
inline uint128 &uint128::operator=(unsigned long v)
{
    return *this = uint128(v);
}

// NOLINTNEXTLINE(runtime/int)
inline uint128 &uint128::operator=(long long v)
{
    return *this = uint128(v);
}

// NOLINTNEXTLINE(runtime/int)
inline uint128 &uint128::operator=(unsigned long long v)
{
    return *this = uint128(v);
}

inline uint128 &uint128::operator=(int128 v)
{
    return *this = uint128(v);
}

// Arithmetic operators.

constexpr uint128 operator<<(uint128 lhs, int amount);
constexpr uint128 operator>>(uint128 lhs, int amount);
constexpr uint128 operator+(uint128 lhs, uint128 rhs);
constexpr uint128 operator-(uint128 lhs, uint128 rhs);
uint128 operator*(uint128 lhs, uint128 rhs);
uint128 operator/(uint128 lhs, uint128 rhs);
uint128 operator%(uint128 lhs, uint128 rhs);

inline uint128 &uint128::operator<<=(int amount)
{
    *this = *this << amount;
    return *this;
}

inline uint128 &uint128::operator>>=(int amount)
{
    *this = *this >> amount;
    return *this;
}

inline uint128 &uint128::operator+=(uint128 other)
{
    *this = *this + other;
    return *this;
}

inline uint128 &uint128::operator-=(uint128 other)
{
    *this = *this - other;
    return *this;
}

inline uint128 &uint128::operator*=(uint128 other)
{
    *this = *this * other;
    return *this;
}

inline uint128 &uint128::operator/=(uint128 other)
{
    *this = *this / other;
    return *this;
}

inline uint128 &uint128::operator%=(uint128 other)
{
    *this = *this % other;
    return *this;
}

constexpr uint64_t Uint128Low64(uint128 v)
{
    return v.lo_;
}

constexpr uint64_t Uint128High64(uint128 v)
{
    return v.hi_;
}

// Constructors from integer types.

#if defined(SYSLABCC_IS_LITTLE_ENDIAN)

constexpr uint128::uint128(uint64_t high, uint64_t low) : lo_{low}, hi_{high}
{
}

constexpr uint128::uint128(int v)
    : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? (std::numeric_limits<uint64_t>::max)() : 0}
{
}
constexpr uint128::uint128(long v) // NOLINT(runtime/int)
    : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? (std::numeric_limits<uint64_t>::max)() : 0}
{
}
constexpr uint128::uint128(long long v) // NOLINT(runtime/int)
    : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? (std::numeric_limits<uint64_t>::max)() : 0}
{
}

constexpr uint128::uint128(unsigned int v) : lo_{v}, hi_{0}
{
}
// NOLINTNEXTLINE(runtime/int)
constexpr uint128::uint128(unsigned long v) : lo_{v}, hi_{0}
{
}
// NOLINTNEXTLINE(runtime/int)
constexpr uint128::uint128(unsigned long long v) : lo_{v}, hi_{0}
{
}

constexpr uint128::uint128(int128 v) : lo_{Int128Low64(v)}, hi_{static_cast<uint64_t>(Int128High64(v))}
{
}

#elif defined(SYSLABCC_IS_BIG_ENDIAN)

constexpr uint128::uint128(uint64_t high, uint64_t low) : hi_{high}, lo_{low}
{
}

constexpr uint128::uint128(int v)
    : hi_{v < 0 ? (std::numeric_limits<uint64_t>::max)() : 0}, lo_{static_cast<uint64_t>(v)}
{
}
constexpr uint128::uint128(long v) // NOLINT(runtime/int)
    : hi_{v < 0 ? (std::numeric_limits<uint64_t>::max)() : 0}, lo_{static_cast<uint64_t>(v)}
{
}
constexpr uint128::uint128(long long v) // NOLINT(runtime/int)
    : hi_{v < 0 ? (std::numeric_limits<uint64_t>::max)() : 0}, lo_{static_cast<uint64_t>(v)}
{
}

constexpr uint128::uint128(unsigned int v) : hi_{0}, lo_{v}
{
}
// NOLINTNEXTLINE(runtime/int)
constexpr uint128::uint128(unsigned long v) : hi_{0}, lo_{v}
{
}
// NOLINTNEXTLINE(runtime/int)
constexpr uint128::uint128(unsigned long long v) : hi_{0}, lo_{v}
{
}

constexpr uint128::uint128(int128 v) : hi_{static_cast<uint64_t>(Int128High64(v))}, lo_{Int128Low64(v)}
{
}

#else // byte order
#error "Unsupported byte order: must be little-endian or big-endian."
#endif // byte order

// Conversion operators to integer types.

constexpr uint128::operator bool() const
{
    return lo_ || hi_;
}

constexpr uint128::operator char() const
{
    return static_cast<char>(lo_);
}

constexpr uint128::operator signed char() const
{
    return static_cast<signed char>(lo_);
}

constexpr uint128::operator unsigned char() const
{
    return static_cast<unsigned char>(lo_);
}

constexpr uint128::operator char16_t() const
{
    return static_cast<char16_t>(lo_);
}

constexpr uint128::operator char32_t() const
{
    return static_cast<char32_t>(lo_);
}

constexpr uint128::operator SYSLABCC_INTERNAL_WCHAR_T() const
{
    return static_cast<SYSLABCC_INTERNAL_WCHAR_T>(lo_);
}

// NOLINTNEXTLINE(runtime/int)
constexpr uint128::operator short() const
{
    return static_cast<short>(lo_);
}

constexpr uint128::operator unsigned short() const
{                                            // NOLINT(runtime/int)
    return static_cast<unsigned short>(lo_); // NOLINT(runtime/int)
}

constexpr uint128::operator int() const
{
    return static_cast<int>(lo_);
}

constexpr uint128::operator unsigned int() const
{
    return static_cast<unsigned int>(lo_);
}

// NOLINTNEXTLINE(runtime/int)
constexpr uint128::operator long() const
{
    return static_cast<long>(lo_);
}

constexpr uint128::operator unsigned long() const
{                                           // NOLINT(runtime/int)
    return static_cast<unsigned long>(lo_); // NOLINT(runtime/int)
}

constexpr uint128::operator long long() const
{                                       // NOLINT(runtime/int)
    return static_cast<long long>(lo_); // NOLINT(runtime/int)
}

constexpr uint128::operator unsigned long long() const
{                                                // NOLINT(runtime/int)
    return static_cast<unsigned long long>(lo_); // NOLINT(runtime/int)
}

// Conversion operators to floating point types.

inline uint128::operator float() const
{
    return static_cast<float>(lo_) + std::ldexp(static_cast<float>(hi_), 64);
}

inline uint128::operator double() const
{
    return static_cast<double>(lo_) + std::ldexp(static_cast<double>(hi_), 64);
}

inline uint128::operator long double() const
{
    return static_cast<long double>(lo_) + std::ldexp(static_cast<long double>(hi_), 64);
}

// Comparison operators.

constexpr bool operator==(uint128 lhs, uint128 rhs)
{

    return (Uint128Low64(lhs) == Uint128Low64(rhs) && Uint128High64(lhs) == Uint128High64(rhs));
}

constexpr bool operator!=(uint128 lhs, uint128 rhs)
{
    return !(lhs == rhs);
}

constexpr bool operator<(uint128 lhs, uint128 rhs)
{

    return (Uint128High64(lhs) == Uint128High64(rhs)) ? (Uint128Low64(lhs) < Uint128Low64(rhs))
                                                      : (Uint128High64(lhs) < Uint128High64(rhs));
}

constexpr bool operator>(uint128 lhs, uint128 rhs)
{
    return rhs < lhs;
}

constexpr bool operator<=(uint128 lhs, uint128 rhs)
{
    return !(rhs < lhs);
}

constexpr bool operator>=(uint128 lhs, uint128 rhs)
{
    return !(lhs < rhs);
}

// Unary operators.

constexpr inline uint128 operator+(uint128 val)
{
    return val;
}

constexpr inline int128 operator+(int128 val)
{
    return val;
}

constexpr uint128 operator-(uint128 val)
{

    return MakeUint128(~Uint128High64(val) + static_cast<unsigned long>(Uint128Low64(val) == 0),
                       ~Uint128Low64(val) + 1);
}

constexpr inline bool operator!(uint128 val)
{

    return !Uint128High64(val) && !Uint128Low64(val);
}

// Logical operators.

constexpr inline uint128 operator~(uint128 val)
{

    return MakeUint128(~Uint128High64(val), ~Uint128Low64(val));
}

constexpr inline uint128 operator|(uint128 lhs, uint128 rhs)
{

    return MakeUint128(Uint128High64(lhs) | Uint128High64(rhs), Uint128Low64(lhs) | Uint128Low64(rhs));
}

constexpr inline uint128 operator&(uint128 lhs, uint128 rhs)
{

    return MakeUint128(Uint128High64(lhs) & Uint128High64(rhs), Uint128Low64(lhs) & Uint128Low64(rhs));
}

constexpr inline uint128 operator^(uint128 lhs, uint128 rhs)
{

    return MakeUint128(Uint128High64(lhs) ^ Uint128High64(rhs), Uint128Low64(lhs) ^ Uint128Low64(rhs));
}

inline uint128 &uint128::operator|=(uint128 other)
{
    *this = *this | other;
    return *this;
}

inline uint128 &uint128::operator&=(uint128 other)
{
    *this = *this & other;
    return *this;
}

inline uint128 &uint128::operator^=(uint128 other)
{
    *this = *this ^ other;
    return *this;
}

// Arithmetic operators.

constexpr uint128 operator<<(uint128 lhs, int amount)
{
    // uint64_t shifts of >= 64 are undefined, so we will need some
    // special-casing.
    return amount >= 64  ? MakeUint128(Uint128Low64(lhs) << (amount - 64), 0)
           : amount == 0 ? lhs
                         : MakeUint128((Uint128High64(lhs) << amount) | (Uint128Low64(lhs) >> (64 - amount)),
                                       Uint128Low64(lhs) << amount);
}

constexpr uint128 operator>>(uint128 lhs, int amount)
{

    // uint64_t shifts of >= 64 are undefined, so we will need some
    // special-casing.
    return amount >= 64  ? MakeUint128(0, Uint128High64(lhs) >> (amount - 64))
           : amount == 0 ? lhs
                         : MakeUint128(Uint128High64(lhs) >> amount,
                                       (Uint128Low64(lhs) >> amount) | (Uint128High64(lhs) << (64 - amount)));
}

namespace int128_internal
{
constexpr uint128 AddResult(uint128 result, uint128 lhs)
{
    // check for carry
    return (Uint128Low64(result) < Uint128Low64(lhs)) ? MakeUint128(Uint128High64(result) + 1, Uint128Low64(result))
                                                      : result;
}
} // namespace int128_internal

constexpr uint128 operator+(uint128 lhs, uint128 rhs)
{

    return int128_internal::AddResult(
        MakeUint128(Uint128High64(lhs) + Uint128High64(rhs), Uint128Low64(lhs) + Uint128Low64(rhs)), lhs);
}

namespace int128_internal
{
constexpr uint128 SubstractResult(uint128 result, uint128 lhs, uint128 rhs)
{
    // check for carry
    return (Uint128Low64(lhs) < Uint128Low64(rhs)) ? MakeUint128(Uint128High64(result) - 1, Uint128Low64(result))
                                                   : result;
}
} // namespace int128_internal

constexpr uint128 operator-(uint128 lhs, uint128 rhs)
{

    return int128_internal::SubstractResult(
        MakeUint128(Uint128High64(lhs) - Uint128High64(rhs), Uint128Low64(lhs) - Uint128Low64(rhs)), lhs, rhs);
}

inline uint128 operator*(uint128 lhs, uint128 rhs)
{

    uint64_t a32 = Uint128Low64(lhs) >> 32;
    uint64_t a00 = Uint128Low64(lhs) & 0xffffffff;
    uint64_t b32 = Uint128Low64(rhs) >> 32;
    uint64_t b00 = Uint128Low64(rhs) & 0xffffffff;
    uint128 result = MakeUint128(
        Uint128High64(lhs) * Uint128Low64(rhs) + Uint128Low64(lhs) * Uint128High64(rhs) + a32 * b32, a00 * b00);
    result += uint128(a32 * b00) << 32;
    result += uint128(a00 * b32) << 32;
    return result;
}

inline uint128 uint128::operator++(int)
{
    uint128 tmp(*this);
    *this += 1;
    return tmp;
}

inline uint128 uint128::operator--(int)
{
    uint128 tmp(*this);
    *this -= 1;
    return tmp;
}

inline uint128 &uint128::operator++()
{
    *this += 1;
    return *this;
}

inline uint128 &uint128::operator--()
{
    *this -= 1;
    return *this;
}

// ==========int128===========================================

constexpr int128 MakeInt128(int64_t high, uint64_t low)
{
    return int128(high, low);
}

// Assignment from integer types.
inline int128 &int128::operator=(int v)
{
    return *this = int128(v);
}

inline int128 &int128::operator=(unsigned int v)
{
    return *this = int128(v);
}

inline int128 &int128::operator=(long v)
{ // NOLINT(runtime/int)
    return *this = int128(v);
}

// NOLINTNEXTLINE(runtime/int)
inline int128 &int128::operator=(unsigned long v)
{
    return *this = int128(v);
}

// NOLINTNEXTLINE(runtime/int)
inline int128 &int128::operator=(long long v)
{
    return *this = int128(v);
}

// NOLINTNEXTLINE(runtime/int)
inline int128 &int128::operator=(unsigned long long v)
{
    return *this = int128(v);
}

// Arithmetic operators.
constexpr int128 operator-(int128 v);
constexpr int128 operator+(int128 lhs, int128 rhs);
constexpr int128 operator-(int128 lhs, int128 rhs);
int128 operator*(int128 lhs, int128 rhs);
int128 operator/(int128 lhs, int128 rhs);
int128 operator%(int128 lhs, int128 rhs);
constexpr int128 operator|(int128 lhs, int128 rhs);
constexpr int128 operator&(int128 lhs, int128 rhs);
constexpr int128 operator^(int128 lhs, int128 rhs);
int128 operator<<(int128 lhs, int amount);
int128 operator>>(int128 lhs, int amount);

inline int128 &int128::operator+=(int128 other)
{
    *this = *this + other;
    return *this;
}

inline int128 &int128::operator-=(int128 other)
{
    *this = *this - other;
    return *this;
}

inline int128 &int128::operator*=(int128 other)
{
    *this = *this * other;
    return *this;
}

inline int128 &int128::operator/=(int128 other)
{
    *this = *this / other;
    return *this;
}

inline int128 &int128::operator%=(int128 other)
{
    *this = *this % other;
    return *this;
}

inline int128 &int128::operator|=(int128 other)
{
    *this = *this | other;
    return *this;
}

inline int128 &int128::operator&=(int128 other)
{
    *this = *this & other;
    return *this;
}

inline int128 &int128::operator^=(int128 other)
{
    *this = *this ^ other;
    return *this;
}

inline int128 &int128::operator<<=(int amount)
{
    *this = *this << amount;
    return *this;
}

inline int128 &int128::operator>>=(int amount)
{
    *this = *this >> amount;
    return *this;
}

// Forward declaration for comparison operators.
constexpr bool operator!=(int128 lhs, int128 rhs);

namespace int128_internal
{

// Casts from unsigned to signed while preserving the underlying binary
// representation.
constexpr int64_t BitCastToSigned(uint64_t v)
{
    // Casting an unsigned integer to a signed integer of the same
    // width is implementation defined behavior if the source value would not fit
    // in the destination type. We step around it with a roundtrip bitwise not
    // operation to make sure this function remains constexpr. Clang, GCC, and
    // MSVC optimize this to a no-op on x86-64.
    return v & (uint64_t{1} << 63) ? ~static_cast<int64_t>(~v) : static_cast<int64_t>(v);
}

} // namespace int128_internal

constexpr uint64_t Int128Low64(int128 v)
{
    return v.lo_;
}

constexpr int64_t Int128High64(int128 v)
{
    return v.hi_;
}

#if defined(SYSLABCC_IS_LITTLE_ENDIAN)

constexpr int128::int128(int64_t high, uint64_t low) : lo_(low), hi_(high)
{
}

constexpr int128::int128(int v) : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? ~int64_t{0} : 0}
{
}
constexpr int128::int128(long v) // NOLINT(runtime/int)
    : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? ~int64_t{0} : 0}
{
}
constexpr int128::int128(long long v) // NOLINT(runtime/int)
    : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? ~int64_t{0} : 0}
{
}

constexpr int128::int128(unsigned int v) : lo_{v}, hi_{0}
{
}
// NOLINTNEXTLINE(runtime/int)
constexpr int128::int128(unsigned long v) : lo_{v}, hi_{0}
{
}
// NOLINTNEXTLINE(runtime/int)
constexpr int128::int128(unsigned long long v) : lo_{v}, hi_{0}
{
}

constexpr int128::int128(uint128 v) : lo_{Uint128Low64(v)}, hi_{static_cast<int64_t>(Uint128High64(v))}
{
}

#elif defined(SYSLABCC_IS_BIG_ENDIAN)

constexpr int128::int128(int64_t high, uint64_t low) : hi_{high}, lo_{low}
{
}

constexpr int128::int128(int v) : hi_{v < 0 ? ~int64_t{0} : 0}, lo_{static_cast<uint64_t>(v)}
{
}
constexpr int128::int128(long v) // NOLINT(runtime/int)
    : hi_{v < 0 ? ~int64_t{0} : 0}, lo_{static_cast<uint64_t>(v)}
{
}
constexpr int128::int128(long long v) // NOLINT(runtime/int)
    : hi_{v < 0 ? ~int64_t{0} : 0}, lo_{static_cast<uint64_t>(v)}
{
}

constexpr int128::int128(unsigned int v) : hi_{0}, lo_{v}
{
}
// NOLINTNEXTLINE(runtime/int)
constexpr int128::int128(unsigned long v) : hi_{0}, lo_{v}
{
}
// NOLINTNEXTLINE(runtime/int)
constexpr int128::int128(unsigned long long v) : hi_{0}, lo_{v}
{
}

constexpr int128::int128(uint128 v) : hi_{static_cast<int64_t>(Uint128High64(v))}, lo_{Uint128Low64(v)}
{
}

#else // byte order
#error "Unsupported byte order: must be little-endian or big-endian."
#endif // byte order

constexpr int128::operator bool() const
{
    return lo_ || hi_;
}

constexpr int128::operator char() const
{
    // NOLINTNEXTLINE(runtime/int)
    return static_cast<char>(static_cast<long long>(*this));
}

constexpr int128::operator signed char() const
{
    // NOLINTNEXTLINE(runtime/int)
    return static_cast<signed char>(static_cast<long long>(*this));
}

constexpr int128::operator unsigned char() const
{
    return static_cast<unsigned char>(lo_);
}

constexpr int128::operator char16_t() const
{
    return static_cast<char16_t>(lo_);
}

constexpr int128::operator char32_t() const
{
    return static_cast<char32_t>(lo_);
}

constexpr int128::operator SYSLABCC_INTERNAL_WCHAR_T() const
{
    // NOLINTNEXTLINE(runtime/int)
    return static_cast<SYSLABCC_INTERNAL_WCHAR_T>(static_cast<long long>(*this));
}

constexpr int128::operator short() const
{ // NOLINT(runtime/int)
    // NOLINTNEXTLINE(runtime/int)
    return static_cast<short>(static_cast<long long>(*this));
}

constexpr int128::operator unsigned short() const
{                                            // NOLINT(runtime/int)
    return static_cast<unsigned short>(lo_); // NOLINT(runtime/int)
}

constexpr int128::operator int() const
{
    // NOLINTNEXTLINE(runtime/int)
    return static_cast<int>(static_cast<long long>(*this));
}

constexpr int128::operator unsigned int() const
{
    return static_cast<unsigned int>(lo_);
}

constexpr int128::operator long() const
{ // NOLINT(runtime/int)
    // NOLINTNEXTLINE(runtime/int)
    return static_cast<long>(static_cast<long long>(*this));
}

constexpr int128::operator unsigned long() const
{                                           // NOLINT(runtime/int)
    return static_cast<unsigned long>(lo_); // NOLINT(runtime/int)
}

constexpr int128::operator long long() const
{ // NOLINT(runtime/int)
    // We don't bother checking the value of hi_. If *this < 0, lo_'s high bit
    // must be set in order for the value to fit into a long long. Conversely, if
    // lo_'s high bit is set, *this must be < 0 for the value to fit.
    return int128_internal::BitCastToSigned(lo_);
}

constexpr int128::operator unsigned long long() const
{                                                // NOLINT(runtime/int)
    return static_cast<unsigned long long>(lo_); // NOLINT(runtime/int)
}

inline int128::operator float() const
{
    // We must convert the absolute value and then negate as needed, because
    // floating point types are typically sign-magnitude. Otherwise, the
    // difference between the high and low 64 bits when interpreted as two's
    // complement overwhelms the precision of the mantissa.
    //
    // Also check to make sure we don't negate Int128Min()
    return hi_ < 0 && *this != Int128Min() ? -static_cast<float>(-*this)
                                           : static_cast<float>(lo_) + std::ldexp(static_cast<float>(hi_), 64);
}

inline int128::operator double() const
{
    // See comment in int128::operator float() above.
    return hi_ < 0 && *this != Int128Min() ? -static_cast<double>(-*this)
                                           : static_cast<double>(lo_) + std::ldexp(static_cast<double>(hi_), 64);
}

inline int128::operator long double() const
{
    // See comment in int128::operator float() above.
    return hi_ < 0 && *this != Int128Min()
               ? -static_cast<long double>(-*this)
               : static_cast<long double>(lo_) + std::ldexp(static_cast<long double>(hi_), 64);
}

// Comparison operators.

constexpr bool operator==(int128 lhs, int128 rhs)
{
    return (Int128Low64(lhs) == Int128Low64(rhs) && Int128High64(lhs) == Int128High64(rhs));
}

constexpr bool operator!=(int128 lhs, int128 rhs)
{
    return !(lhs == rhs);
}

constexpr bool operator<(int128 lhs, int128 rhs)
{
    return (Int128High64(lhs) == Int128High64(rhs)) ? (Int128Low64(lhs) < Int128Low64(rhs))
                                                    : (Int128High64(lhs) < Int128High64(rhs));
}

constexpr bool operator>(int128 lhs, int128 rhs)
{
    return (Int128High64(lhs) == Int128High64(rhs)) ? (Int128Low64(lhs) > Int128Low64(rhs))
                                                    : (Int128High64(lhs) > Int128High64(rhs));
}

constexpr bool operator<=(int128 lhs, int128 rhs)
{
    return !(lhs > rhs);
}

constexpr bool operator>=(int128 lhs, int128 rhs)
{
    return !(lhs < rhs);
}

// Unary operators.

constexpr int128 operator-(int128 v)
{
    return MakeInt128(~Int128High64(v) + (Int128Low64(v) == 0), ~Int128Low64(v) + 1);
}

constexpr bool operator!(int128 v)
{
    return !Int128Low64(v) && !Int128High64(v);
}

constexpr int128 operator~(int128 val)
{
    return MakeInt128(~Int128High64(val), ~Int128Low64(val));
}

// Arithmetic operators.

namespace int128_internal
{
constexpr int128 SignedAddResult(int128 result, int128 lhs)
{
    // check for carry
    return (Int128Low64(result) < Int128Low64(lhs)) ? MakeInt128(Int128High64(result) + 1, Int128Low64(result))
                                                    : result;
}
} // namespace int128_internal
constexpr int128 operator+(int128 lhs, int128 rhs)
{
    return int128_internal::SignedAddResult(
        MakeInt128(Int128High64(lhs) + Int128High64(rhs), Int128Low64(lhs) + Int128Low64(rhs)), lhs);
}

namespace int128_internal
{
constexpr int128 SignedSubstractResult(int128 result, int128 lhs, int128 rhs)
{
    // check for carry
    return (Int128Low64(lhs) < Int128Low64(rhs)) ? MakeInt128(Int128High64(result) - 1, Int128Low64(result)) : result;
}
} // namespace int128_internal
constexpr int128 operator-(int128 lhs, int128 rhs)
{
    return int128_internal::SignedSubstractResult(
        MakeInt128(Int128High64(lhs) - Int128High64(rhs), Int128Low64(lhs) - Int128Low64(rhs)), lhs, rhs);
}

inline int128 operator*(int128 lhs, int128 rhs)
{
    return MakeInt128(int128_internal::BitCastToSigned(Uint128High64(uint128(lhs) * rhs)),
                      Uint128Low64(uint128(lhs) * rhs));
}

inline int128 int128::operator++(int)
{
    int128 tmp(*this);
    *this += 1;
    return tmp;
}

inline int128 int128::operator--(int)
{
    int128 tmp(*this);
    *this -= 1;
    return tmp;
}

inline int128 &int128::operator++()
{
    *this += 1;
    return *this;
}

inline int128 &int128::operator--()
{
    *this -= 1;
    return *this;
}

constexpr int128 operator|(int128 lhs, int128 rhs)
{
    return MakeInt128(Int128High64(lhs) | Int128High64(rhs), Int128Low64(lhs) | Int128Low64(rhs));
}

constexpr int128 operator&(int128 lhs, int128 rhs)
{
    return MakeInt128(Int128High64(lhs) & Int128High64(rhs), Int128Low64(lhs) & Int128Low64(rhs));
}

constexpr int128 operator^(int128 lhs, int128 rhs)
{
    return MakeInt128(Int128High64(lhs) ^ Int128High64(rhs), Int128Low64(lhs) ^ Int128Low64(rhs));
}

int128 operator<<(int128 lhs, int amount)
{
    // int64_t shifts of >= 63 are undefined, so we need some special-casing.
    assert(amount >= 0 && amount < 127);
    if (amount <= 0)
    {
        return lhs;
    }
    else if (amount < 63)
    {
        return MakeInt128((Int128High64(lhs) << amount) | static_cast<int64_t>(Int128Low64(lhs) >> (64 - amount)),
                          Int128Low64(lhs) << amount);
    }
    else if (amount == 63)
    {
        return MakeInt128(((Int128High64(lhs) << 32) << 31) | static_cast<int64_t>(Int128Low64(lhs) >> 1),
                          (Int128Low64(lhs) << 32) << 31);
    }
    else if (amount == 127)
    {
        return MakeInt128(static_cast<int64_t>(Int128Low64(lhs) << 63), 0);
    }
    else if (amount > 127)
    {
        return MakeInt128(0, 0);
    }
    else
    {
        // amount >= 64 && amount < 127
        return MakeInt128(static_cast<int64_t>(Int128Low64(lhs) << (amount - 64)), 0);
    }
}

int128 operator>>(int128 lhs, int amount)
{
    // int64_t shifts of >= 63 are undefined, so we need some special-casing.
    assert(amount >= 0 && amount < 127);
    if (amount <= 0)
    {
        return lhs;
    }
    else if (amount < 63)
    {
        return MakeInt128(Int128High64(lhs) >> amount,
                          Int128Low64(lhs) >> amount | static_cast<uint64_t>(Int128High64(lhs)) << (64 - amount));
    }
    else if (amount == 63)
    {
        return MakeInt128((Int128High64(lhs) >> 32) >> 31,
                          static_cast<uint64_t>(Int128High64(lhs) << 1) | (Int128Low64(lhs) >> 32) >> 31);
    }
    else if (amount >= 127)
    {
        return MakeInt128((Int128High64(lhs) >> 32) >> 31, static_cast<uint64_t>((Int128High64(lhs) >> 32) >> 31));
    }
    else
    {
        // amount >= 64 && amount < 127
        return MakeInt128((Int128High64(lhs) >> 32) >> 31, static_cast<uint64_t>(Int128High64(lhs) >> (amount - 64)));
    }
}

// =========int128.cc=====================

// A function-like feature checking macro that is a wrapper around
// `__has_attribute`, which is defined by GCC 5+ and Clang and evaluates to a
// nonzero constant integer if the attribute is supported or 0 if not.
//
// It evaluates to zero if `__has_attribute` is not defined by the compiler.
//
// GCC: https://gcc.gnu.org/gcc-5/changes.html
// Clang: https://clang.llvm.org/docs/LanguageExtensions.html
#ifdef __has_attribute
#define SYSLABCC_HAVE_ATTRIBUTE(x) __has_attribute(x)
#else
#define SYSLABCC_HAVE_ATTRIBUTE(x) 0
#endif

#ifdef __has_builtin
#define SYSLABCC_HAVE_BUILTIN(x) __has_builtin(x)
#else
#define SYSLABCC_HAVE_BUILTIN(x) 0
#endif

// SYSLABCC_ATTRIBUTE_ALWAYS_INLINE
// SYSLABCC_ATTRIBUTE_NOINLINE
//
// Forces functions to either inline or not inline. Introduced in gcc 3.1.
#if SYSLABCC_HAVE_ATTRIBUTE(always_inline) || (defined(__GNUC__) && !defined(__clang__))
#define SYSLABCC_ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#define SYSLABCC_HAVE_ATTRIBUTE_ALWAYS_INLINE 1
#else
#define SYSLABCC_ATTRIBUTE_ALWAYS_INLINE
#endif

#if SYSLABCC_HAVE_ATTRIBUTE(noinline) || (defined(__GNUC__) && !defined(__clang__))
#define SYSLABCC_ATTRIBUTE_NOINLINE __attribute__((noinline))
#define SYSLABCC_HAVE_ATTRIBUTE_NOINLINE 1
#else
#define SYSLABCC_ATTRIBUTE_NOINLINE
#endif

#if !defined(NDEBUG)
#define SYSLABCC_ASSUME(cond) assert(cond)
#elif SYSLABCC_HAVE_BUILTIN(__builtin_assume)
#define SYSLABCC_ASSUME(cond) __builtin_assume(cond)
#elif defined(_MSC_VER)
#define SYSLABCC_ASSUME(cond) __assume(cond)
#elif defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
#define SYSLABCC_ASSUME(cond)                                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(cond))                                                                                                   \
            std::unreachable();                                                                                        \
    } while (false)
#elif defined(__GNUC__) || SYSLABCC_HAVE_BUILTIN(__builtin_unreachable)
#define SYSLABCC_ASSUME(cond)                                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(cond))                                                                                                   \
            __builtin_unreachable();                                                                                   \
    } while (false)
#else
#define SYSLABCC_ASSUME(cond)                                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        static_cast<void>(false && (cond));                                                                            \
    } while (false)
#endif

namespace
{

// Returns the 0-based position of the last set bit (i.e., most significant bit)
// in the given uint128. The argument is not 0.
//
// For example:
//   Given: 5 (decimal) == 101 (binary)
//   Returns: 2
inline SYSLABCC_ATTRIBUTE_ALWAYS_INLINE int Fls128(uint128 n)
{
    if (uint64_t hi = Uint128High64(n))
    {
        SYSLABCC_ASSUME(hi != 0);
        static_assert(sizeof(unsigned long long) == sizeof(uint64_t),
                      "_sj_ctlz_int: sizeof(unsigned long long) == sizeof(uint64_t)");
        auto ret = __builtin_clzll(hi);
        return 127 - ret;
    }
    const uint64_t low = Uint128Low64(n);
    SYSLABCC_ASSUME(low != 0);
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t),
                  "_sj_ctlz_int: sizeof(unsigned long long) == sizeof(uint64_t)");
    auto ret = __builtin_clzll(low);
    return 63 - ret;
}

// Long division/modulo for uint128 implemented using the shift-subtract
// division algorithm adapted from:
// https://stackoverflow.com/questions/5386377/division-without-using
inline void DivModImpl(uint128 dividend, uint128 divisor, uint128 *quotient_ret, uint128 *remainder_ret)
{
    assert(divisor != 0);

    if (divisor > dividend)
    {
        *quotient_ret = 0;
        *remainder_ret = dividend;
        return;
    }

    if (divisor == dividend)
    {
        *quotient_ret = 1;
        *remainder_ret = 0;
        return;
    }

    uint128 denominator = divisor;
    uint128 quotient = 0;

    // Left aligns the MSB of the denominator and the dividend.
    const int shift = Fls128(dividend) - Fls128(denominator);
    denominator <<= shift;

    // Uses shift-subtract algorithm to divide dividend by denominator. The
    // remainder will be left in dividend.
    for (int i = 0; i <= shift; ++i)
    {
        quotient <<= 1;
        if (dividend >= denominator)
        {
            dividend -= denominator;
            quotient |= 1;
        }
        denominator >>= 1;
    }

    *quotient_ret = quotient;
    *remainder_ret = dividend;
}

template <typename T> uint128 MakeUint128FromFloat(T v)
{
    static_assert(std::is_floating_point<T>::value, "");

    // Rounding behavior is towards zero, same as for built-in types.

    // Undefined behavior if v is NaN or cannot fit into uint128.
    assert(std::isfinite(v) && v > -1 &&
           (std::numeric_limits<T>::max_exponent <= 128 || v < std::ldexp(static_cast<T>(1), 128)));

    if (v >= std::ldexp(static_cast<T>(1), 64))
    {
        uint64_t hi = static_cast<uint64_t>(std::ldexp(v, -64));
        uint64_t lo = static_cast<uint64_t>(v - std::ldexp(static_cast<T>(hi), 64));
        return MakeUint128(hi, lo);
    }

    return MakeUint128(0, static_cast<uint64_t>(v));
}

#if defined(__clang__) && (__clang_major__ < 9) && !defined(__SSE3__)
// Workaround for clang bug: https://bugs.llvm.org/show_bug.cgi?id=38289
// Casting from long double to uint64_t is miscompiled and drops bits.
// It is more work, so only use when we need the workaround.
uint128 MakeUint128FromFloat(long double v)
{
    // Go 50 bits at a time, that fits in a double
    static_assert(std::numeric_limits<double>::digits >= 50, "");
    static_assert(std::numeric_limits<long double>::digits <= 150, "");
    // Undefined behavior if v is not finite or cannot fit into uint128.
    assert(std::isfinite(v) && v > -1 && v < std::ldexp(1.0L, 128));

    v = std::ldexp(v, -100);
    uint64_t w0 = static_cast<uint64_t>(static_cast<double>(std::trunc(v)));
    v = std::ldexp(v - static_cast<double>(w0), 50);
    uint64_t w1 = static_cast<uint64_t>(static_cast<double>(std::trunc(v)));
    v = std::ldexp(v - static_cast<double>(w1), 50);
    uint64_t w2 = static_cast<uint64_t>(static_cast<double>(std::trunc(v)));
    return (static_cast<uint128>(w0) << 100) | (static_cast<uint128>(w1) << 50) | static_cast<uint128>(w2);
}
#endif // __clang__ && (__clang_major__ < 9) && !__SSE3__
} // namespace

uint128::uint128(float v) : uint128(MakeUint128FromFloat(v))
{
}
uint128::uint128(double v) : uint128(MakeUint128FromFloat(v))
{
}
uint128::uint128(long double v) : uint128(MakeUint128FromFloat(v))
{
}

uint128 operator/(uint128 lhs, uint128 rhs)
{
    uint128 quotient = 0;
    uint128 remainder = 0;
    DivModImpl(lhs, rhs, &quotient, &remainder);
    return quotient;
}

uint128 operator%(uint128 lhs, uint128 rhs)
{
    uint128 quotient = 0;
    uint128 remainder = 0;
    DivModImpl(lhs, rhs, &quotient, &remainder);
    return remainder;
}

namespace
{

std::string Uint128ToFormattedString(uint128 v, std::ios_base::fmtflags flags)
{
    // Select a divisor which is the largest power of the base < 2^64.
    uint128 div;
    int div_base_log;
    switch (flags & std::ios::basefield)
    {
    case std::ios::hex:
        div = 0x1000000000000000; // 16^15
        div_base_log = 15;
        break;
    case std::ios::oct:
        div = 01000000000000000000000; // 8^21
        div_base_log = 21;
        break;
    default:                         // std::ios::dec
        div = 10000000000000000000u; // 10^19
        div_base_log = 19;
        break;
    }

    // Now piece together the uint128 representation from three chunks of the
    // original value, each less than "div" and therefore representable as a
    // uint64_t.
    std::ostringstream os;
    std::ios_base::fmtflags copy_mask = std::ios::basefield | std::ios::showbase | std::ios::uppercase;
    os.setf(flags & copy_mask, copy_mask);
    uint128 high = v;
    uint128 low;
    DivModImpl(high, div, &high, &low);
    uint128 mid;
    DivModImpl(high, div, &high, &mid);
    if (Uint128Low64(high) != 0)
    {
        os << Uint128Low64(high);
        os << std::noshowbase << std::setfill('0') << std::setw(div_base_log);
        os << Uint128Low64(mid);
        os << std::setw(div_base_log);
    }
    else if (Uint128Low64(mid) != 0)
    {
        os << Uint128Low64(mid);
        os << std::noshowbase << std::setfill('0') << std::setw(div_base_log);
    }
    os << Uint128Low64(low);
    return os.str();
}

} // namespace

std::string uint128::ToString() const
{
    return Uint128ToFormattedString(*this, std::ios_base::dec);
}

std::ostream &operator<<(std::ostream &os, uint128 v)
{
    std::ios_base::fmtflags flags = os.flags();
    std::string rep = Uint128ToFormattedString(v, flags);

    // Add the requisite padding.
    std::streamsize width = os.width(0);
    if (static_cast<size_t>(width) > rep.size())
    {
        const size_t count = static_cast<size_t>(width) - rep.size();
        std::ios::fmtflags adjustfield = flags & std::ios::adjustfield;
        if (adjustfield == std::ios::left)
        {
            rep.append(count, os.fill());
        }
        else if (adjustfield == std::ios::internal && (flags & std::ios::showbase) &&
                 (flags & std::ios::basefield) == std::ios::hex && v != 0)
        {
            rep.insert(size_t{2}, count, os.fill());
        }
        else
        {
            rep.insert(size_t{0}, count, os.fill());
        }
    }

    return os << rep;
}

namespace
{

uint128 UnsignedAbsoluteValue(int128 v)
{
    // Cast to uint128 before possibly negating because -Int128Min() is undefined.
    return Int128High64(v) < 0 ? -uint128(v) : uint128(v);
}

} // namespace

namespace
{

template <typename T> int128 MakeInt128FromFloat(T v)
{
    // Conversion when v is NaN or cannot fit into int128 would be undefined
    // behavior if using an intrinsic 128-bit integer.
    assert(std::isfinite(v) && (std::numeric_limits<T>::max_exponent <= 127 ||
                                (v >= -std::ldexp(static_cast<T>(1), 127) && v < std::ldexp(static_cast<T>(1), 127))));

    // We must convert the absolute value and then negate as needed, because
    // floating point types are typically sign-magnitude. Otherwise, the
    // difference between the high and low 64 bits when interpreted as two's
    // complement overwhelms the precision of the mantissa.
    uint128 result = v < 0 ? -MakeUint128FromFloat(-v) : MakeUint128FromFloat(v);
    return MakeInt128(int128_internal::BitCastToSigned(Uint128High64(result)), Uint128Low64(result));
}

} // namespace

int128::int128(float v) : int128(MakeInt128FromFloat(v))
{
}
int128::int128(double v) : int128(MakeInt128FromFloat(v))
{
}
int128::int128(long double v) : int128(MakeInt128FromFloat(v))
{
}

int128 operator/(int128 lhs, int128 rhs)
{
    assert(lhs != Int128Min() || rhs != -1); // UB on two's complement.

    uint128 quotient = 0;
    uint128 remainder = 0;
    DivModImpl(UnsignedAbsoluteValue(lhs), UnsignedAbsoluteValue(rhs), &quotient, &remainder);
    if ((Int128High64(lhs) < 0) != (Int128High64(rhs) < 0))
        quotient = -quotient;
    return MakeInt128(int128_internal::BitCastToSigned(Uint128High64(quotient)), Uint128Low64(quotient));
}

int128 operator%(int128 lhs, int128 rhs)
{
    assert(lhs != Int128Min() || rhs != -1); // UB on two's complement.

    uint128 quotient = 0;
    uint128 remainder = 0;
    DivModImpl(UnsignedAbsoluteValue(lhs), UnsignedAbsoluteValue(rhs), &quotient, &remainder);
    if (Int128High64(lhs) < 0)
        remainder = -remainder;
    return MakeInt128(int128_internal::BitCastToSigned(Uint128High64(remainder)), Uint128Low64(remainder));
}

std::string int128::ToString() const
{
    std::string rep;
    if (Int128High64(*this) < 0)
        rep = "-";
    rep.append(Uint128ToFormattedString(UnsignedAbsoluteValue(*this), std::ios_base::dec));
    return rep;
}

std::ostream &operator<<(std::ostream &os, int128 v)
{
    std::ios_base::fmtflags flags = os.flags();
    std::string rep;

    // Add the sign if needed.
    bool print_as_decimal =
        (flags & std::ios::basefield) == std::ios::dec || (flags & std::ios::basefield) == std::ios_base::fmtflags();
    if (print_as_decimal)
    {
        if (Int128High64(v) < 0)
        {
            rep = "-";
        }
        else if (flags & std::ios::showpos)
        {
            rep = "+";
        }
    }

    rep.append(Uint128ToFormattedString(print_as_decimal ? UnsignedAbsoluteValue(v) : uint128(v), os.flags()));

    // Add the requisite padding.
    std::streamsize width = os.width(0);
    if (static_cast<size_t>(width) > rep.size())
    {
        const size_t count = static_cast<size_t>(width) - rep.size();
        switch (flags & std::ios::adjustfield)
        {
        case std::ios::left:
            rep.append(count, os.fill());
            break;
        case std::ios::internal:
            if (print_as_decimal && (rep[0] == '+' || rep[0] == '-'))
            {
                rep.insert(size_t{1}, count, os.fill());
            }
            else if ((flags & std::ios::basefield) == std::ios::hex && (flags & std::ios::showbase) && v != 0)
            {
                rep.insert(size_t{2}, count, os.fill());
            }
            else
            {
                rep.insert(size_t{0}, count, os.fill());
            }
            break;
        default: // std::ios::right
            rep.insert(size_t{0}, count, os.fill());
            break;
        }
    }

    return os << rep;
}

} // namespace intrinsics
} // namespace rt
} // namespace syslab

#undef SYSLABCC_INTERNAL_WCHAR_T

#endif // SYSLABCC_HAVE_INTRINSIC_INT128

#endif // SYSLABCRT_DEF_INTRIN_INT128_HPP