#ifndef SYSLABCRT_DECL_INTRIN_ERRORS_HPP
#define SYSLABCRT_DECL_INTRIN_ERRORS_HPP

#include "syslabcrt-intrinsics/declarations/compat.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/objects.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/stableptr.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/trace.hpp"
#include <cstdint>
#ifdef SYSLABCC_DEBUG_TRACE
#define _SJ_CALLSTACK_SET_ITEM(offset, len) _current_thread_context_ptr.Raw->_sj_callstack_set_item(offset, len);
#define _SJ_CALLSTACK_PUSH _current_thread_context_ptr.Raw->_sj_callstack_push();
#define _SJ_CALLSTACK_POP _current_thread_context_ptr.Raw->_sj_callstack_pop();
#define _SJ_CALLSTACK_INIT _sj_debugtrace_init(_sj_callstack_debug_str, _sj_callstack_debug_frames);
#define _SJ_DECL_CALLSTACK_FRAME                                                                                       \
    _sj_stack_frame_t _CALLSTACK_FRAME(_current_thread_context_ptr.Raw->sj_get_frame_index());
#define _SJ_CALLSTACK_STORE _current_thread_context_ptr.Raw->_sj_callstack_store(_CALLSTACK_FRAME);
#define _SJ_CALLSTACK_CLEAR _current_thread_context_ptr.Raw->_sj_callstack_clear();
#else
#define _SJ_CALLSTACK_SET_ITEM(offset, len)
#define _SJ_CALLSTACK_PUSH
#define _SJ_CALLSTACK_POP
#define _SJ_CALLSTACK_INIT
#define _SJ_DECL_CALLSTACK_FRAME
#define _SJ_CALLSTACK_STORE
#define _SJ_CALLSTACK_CLEAR
#endif
#define SYSLABCC_PRINTBUF_PATH_SIZE 2048
#define SYSLABCC_PRINTBUF_FUNC_SIZE 1024
namespace syslab
{
namespace rt
{

struct _sj_exception_stack
{
    sj_any_t exception;
    Trace::Backtrace backtrace;
    _sj_exception_stack *next;
};

struct _sj_exception_stackroot
{
    _sj_exception_stack *binding;
};

struct _sj_frame_t
{
    std::int64_t index;
    std::int64_t len;
};

struct _sj_stack_frame_t
{
    std::size_t index;
    _sj_stack_frame_t(std::size_t _index) : index(_index) {};
};

enum _sj_debuginfo_decode_one_item_status
{
    Fail = -1,
    Success = 0
};

// 解码到每组数据中的第几个
enum _sj_debuginfo_msg_index
{
    FileNameStart = 0,
    FileNameLen = 1,
    FuncNameStart = 2,
    FuncNameLen = 3,
    LineNumber = 4
};

const std::int64_t _SJ_DEBUGINFO_DECODE_NUMBER_MAX_SIZE = 5;

// 数字变长解码状态结构体
struct _sj_debuginfo_decode_state
{
  public:
    const std::int64_t end;                                  // 结束
    std::int64_t cursor;                                     // 当前解码的位置
    std::uint32_t msg[_SJ_DEBUGINFO_DECODE_NUMBER_MAX_SIZE]; // output msg
    _sj_debuginfo_decode_one_item_status is_decode_success;  // 每次解码成功与否 0 成功 1 失败
    _sj_debuginfo_decode_state(std::int64_t _start, std::int64_t _end)
        : end(_end), cursor(_start), is_decode_success(Success) {};
};

namespace intrinsics
{
static std::int32_t _sj_block_target(_sj_block_t const &block);

static void _sj_block_push(_sj_block_t *block, std::int32_t unwind_target);

static void _sj_block_pop(_sj_block_t *block);

static std::int32_t _sj_errno();

static void _sj_set_errno(std::int32_t value);

static sj_any_t createException(ptr_t<std::uint8_t> jlstr);

static sj_any_t _sj_c_exception_to_jlval(const std::exception &e);

static u8string_t _sj_generic_short_repr_as_string(sj_any_t val);

static _sj_builtintype_bot sj_throw(sj_any_t value);
} // namespace intrinsics
} // namespace rt
} // namespace syslab

#endif
