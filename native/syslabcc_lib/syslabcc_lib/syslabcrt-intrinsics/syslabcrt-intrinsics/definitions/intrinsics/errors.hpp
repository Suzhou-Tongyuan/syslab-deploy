#ifndef SYSLABCRT_DEF_INTRIN_ERRORS_HPP
#define SYSLABCRT_DEF_INTRIN_ERRORS_HPP

#include "syslabcrt-intrinsics/declarations/mechanisms/objects.hpp"
#include "syslabcrt-intrinsics/definitions/intrinsics/io.hpp"
#include "syslabcrt-intrinsics/definitions/mod-pre.hpp"

#include <cstdint>
#include <stdint.h>
#include <stdio.h>

namespace syslab
{
namespace rt
{
namespace intrinsics
{

static std::int32_t _sj_block_target(_sj_block_t const &block)
{
    if (block.top == nullptr)
        return 0;

    return block.top->unwind_target;
}

static void _sj_block_push(_sj_block_t *block, std::int32_t unwind_target)
{
    auto itemPtr = (_sj_block_item_t *)calloc(1, sizeof(_sj_block_item_t));
    itemPtr->next = block->top;
    itemPtr->unwind_target = unwind_target;
    block->top = itemPtr;
}

static void _sj_block_pop(_sj_block_t *block)
{
    auto itemPtr = block->top;
    block->top = itemPtr->next;
    free(itemPtr);
}

static std::int32_t _sj_errno_value = 0;

static std::int32_t _sj_errno()
{
    return _sj_errno_value;
}

static void _sj_set_errno(std::int32_t value)
{
    _sj_errno_value = value;
}

template <typename T> static std::uint8_t _sj_upsilon_isset(_sj_upsilon_t<T> const &res)
{
    return res.contents != nullptr ? 1 : 0;
}

template <typename T> static T _sj_upsilon_get(_sj_upsilon_t<T> const &res)
{
    return res.contents != nullptr ? *res.contents : T();
}

template <typename T> static _sj_upsilon_t<T> _sj_upsilon_new(T const &value)
{
    _sj_upsilon_t<T> ret;
    ret.contents = new T(value);
    return ret;
}

static sj_any_t createException(ptr_t<std::uint8_t> jlstr)
{
    auto msg = intrinsics::sj_box_value(sj_builtin_typeids::String(), jlstr);
    _sj_builtintype_ErrorException exc;
    exc.msg = msg;
    return intrinsics::sj_box_value(sj_builtin_typeids::ErrorException(), exc);
}

static sj_any_t _sj_c_exception_to_jlval(const std::exception &e)
{
    const char *message = e.what();
    ptr_t<std::uint8_t> jlMsg = sj_cstr_to_string((std::uint8_t *)message);
    return createException(jlMsg);
}

static u8string_t _sj_generic_short_repr_as_string(sj_any_t val)
{
    auto info = sj_object_inspect<sj_any_t>(val);
    u8string_t ret((u8char_t *)_sj_string_data(info));
    return ret;
}

static _sj_builtintype_bot sj_throw(sj_any_t value)
{
    throw SJException(value);
}

} // namespace intrinsics
} // namespace rt
} // namespace syslab

#endif // SYSLABCRT_DEF_INTRIN_ERRORS_HPP
