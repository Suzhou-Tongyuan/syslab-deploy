#ifndef SYSLABCRT_DECL_COMPAT_HPP
#define SYSLABCRT_DECL_COMPAT_HPP

#include <string>
#ifdef _MSC_VER
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif
// 有了这个头文件，我们就可以在自己的测试环节使用尽可能现代的编译器来排查代码生成器
// 的各种问题，而用户的古董编译器可以直接无视这些鬼画符

#if defined(SYSLAB_CC_FORCE_CXX98) || __cplusplus <= 201103L
#define CXX11_OVERRIDE
#define CXX11_FINAL
#define CXX11_CONSTEXPR
#define CXX11_NOEXCEPT
#define CXX11_NOEXCEPT_IF(X)
#define CXX20_REQUIRES(X)
#else
#define CXX11_OVERRIDE override
#define CXX11_FINAL final
#define CXX11_CONSTEXPR constexpr
#define CXX11_NOEXCEPT noexcept
#define CXX11_NOEXCEPT_IF(X) noexcept(X)
#if __cplusplus >= 202002L
#define CXX20_REQUIRES(X) requires(X)
#else
#define CXX20_REQUIRES(X)
#endif
#endif

#if defined(SYSLAB_CC_FORCE_CXX98) || __cplusplus < 202002L
typedef char u8char_t;
typedef std::string u8string_t;
#else
typedef char8_t u8char_t;
typedef std::u8string u8string_t;
#endif

#ifndef GCC_VERSION
#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif
#endif

#if !defined(_MSC_VER) && !defined(__clang__) && defined(__GNUC__) && defined(GCC_VERSION) && (GCC_VERSION < 50400)
#define SYSLABCC_LEGACY_GCC
#endif

#if defined(_WIN32) && defined(SYSLABCC_LEGACY_GCC)
#define SYSLABCRT_WIN_THREAD_POOL
#endif

#endif
