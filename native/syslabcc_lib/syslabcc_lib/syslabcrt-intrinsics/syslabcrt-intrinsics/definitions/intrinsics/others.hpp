#ifndef SYSLABCRT_DEF_INTRIN_OTHERS_HPP
#define SYSLABCRT_DEF_INTRIN_OTHERS_HPP
#include "syslabcrt-intrinsics/definitions/mod-pre.hpp"

#include <time.h>

namespace syslab
{
namespace rt
{
namespace intrinsics
{

template <typename T> static T _sj_unsupported_copyast(sj_string_t s, T src)
{
    // convert julia's string to native string
    u8string_t msg = utils::stringFromJulia(s);
    throw Errors::InvalidCopyAst(msg);
}

template <typename T> static T _sj_cast_cstring_to(void *ptr)
{
    // T is Julia's Cstring, sizeof(T) == 8
    ptr_t<uint8_t> temp = ptr_t<uint8_t>(ptr);
    return *reinterpret_cast<T *>(&temp);
}

static ptr_t<std::uint8_t> sj_get_ptls_states()
{
    return ptr_t<std::uint8_t>{nullptr};
}

static double sj_clock_now()
{
    clock_t current = clock();
    return ((double)current) / CLOCKS_PER_SEC;
}

static inline ptr_t<uint8_t> sj_get_host_os()
{
#if defined(__FreeBSD__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"FreeBSD"));
#elif defined(__linux__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"Linux"));
#elif defined(_WIN32) || defined(_WIN64) /* mingw 自带的标志 */ || defined(_MSC_VER) /* MSVC */
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"Windows"));
#elif defined(__APPLE__) && defined(__MACH__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"Darwin"));
#elif defined(__EMSCRIPTEN__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"Emscripten"));
#else
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"UNKNOWN"));
#endif
}

static inline ptr_t<uint8_t> sj_get_host_arch()
{
    // 参考：https://stackoverflow.com/a/66249936/13918782
#if defined(__x86_64__) || defined(_M_X64)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"x86_64"));
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"x86_32"));
#elif defined(__ARM_ARCH_2__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM2"));
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM3"));
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM4T"));
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM5"));
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM6T2"));
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) ||   \
    defined(__ARM_ARCH_6ZK__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM6"));
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||   \
    defined(__ARM_ARCH_7S__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM7"));
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM7A"));
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM7R"));
#elif defined(__ARM_ARCH_7M__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM7M"));
#elif defined(__ARM_ARCH_7S__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM7S"));
#elif defined(__aarch64__) || defined(_M_ARM64)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"ARM64"));
#elif defined(mips) || defined(__mips__) || defined(__mips)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"MIPS"));
#elif defined(__sh__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"SUPERH"));
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) ||                  \
    defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"POWERPC"));
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"POWERPC64"));
#elif defined(__sparc__) || defined(__sparc)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"SPARC"));
#elif defined(__m68k__)
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"M68K"));
#else
    return sj_cstr_to_string(ptr_t<uint8_t>((void *)u8"UNKNOWN"));
#endif
}

} // namespace intrinsics
} // namespace rt
} // namespace syslab

#endif // SYSLABCRT_DEF_INTRIN_OTHERS_HPP