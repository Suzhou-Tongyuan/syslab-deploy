#ifndef SYSLABCRT_DECL_INTRIN_THREAD_CONTEXT_HPP
#define SYSLABCRT_DECL_INTRIN_THREAD_CONTEXT_HPP

#include "syslabcrt-intrinsics/declarations/intrinsics/errors.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/objects.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/stableptr.hpp"
#include <cstddef>
#include <cstdint>

#include <sstream>
#define SYSLABCC_PRINTBUF_PATH_SIZE 2048
#define SYSLABCC_PRINTBUF_FUNC_SIZE 1024

#ifndef _WIN32
#include <mutex>
#endif
#include <vector>

#ifdef SYSLABCC_THREADPOOLSIZE
#else
#define SYSLABCC_THREADPOOLSIZE 4LL
#endif

namespace syslab
{

namespace rt
{

namespace intrinsics
{
inline void _sj_debugtrace_init(const char *src_str, const uint8_t *src_frame);
std::int64_t CXX11_CONSTEXPR _sj_thread_pool_size();
struct _sj_thread_context
{
  public:
    _sj_thread_context(int64_t id) : sj_context_id(id), _sj_errno_value(0), _exception_root(nullptr)
    {
    }

    // thread-local 的异常栈信息
    void _sj_exception_push(const std::exception &e);

    void _sj_exception_pop();

    sj_any_t _sj_exception_get();

    _sj_exception_stackroot *_sj_get_exception_root();

    void _sj_exception_push_jlval(sj_any_t exc);

    _sj_builtintype_bot sj_rethrow();

    _sj_builtintype_bot sj_rethrow_other(sj_any_t exc);

    _sj_exception_stack *_sj_exception_current_item();

    void _sj_callstack_init(std::uint8_t option);

    void _sj_callstack_push();
    void _sj_callstack_pop();

    unused_t _sj_callstack_store(_sj_stack_frame_t &st);

    unused_t _sj_callstack_clear();

    unused_t _sj_callstack_set_item(std::int64_t index, std::int64_t len);

    unused_t sj_print_debugtrace();

    unused_t sj_print_trace();

    ptr_t<std::uint8_t> sj_sprint_trace();

    _sj_stack_frame_t sj_get_frame_index();

    ptr_t<uint8_t> sj_string_of_debugtrace();

    std::int32_t _sj_errno();

    void _sj_set_errno(std::int32_t value);

    unused_t _sj_print_decode_frame(std::int64_t start, std::int64_t len, std::ostringstream *ptr_oss);

    void _sj_decode_one_item(_sj_debuginfo_decode_state &state);

  private:
    std::int64_t sj_context_id;
    std::int32_t _sj_errno_value;
    _sj_exception_stackroot *_exception_root;
    /// 存储正常函数调用的栈
    std::vector<_sj_frame_t> _sj_callstack;
    /// 存储异常的栈信息
    std::vector<_sj_frame_t> _sj_error_callstack;
};

struct _sj_managed_thread_context
{
    ptr_t<_sj_thread_context> context;
    _sj_managed_thread_context();
    ~_sj_managed_thread_context();
    _sj_managed_thread_context(_sj_managed_thread_context &&);
    _sj_managed_thread_context(const _sj_managed_thread_context &) = delete;
    _sj_managed_thread_context &operator=(const _sj_managed_thread_context &) = delete;
};

// 多线程运行thread_for支持
unused_t _sj_threadfor_run(ptr_t<sj_array_t> ptr_fun, ptr_t<sj_array_t> ptr_closure);
std::size_t _sj_get_current_threadid(void);
std::size_t _sj_get_current_taskid(void);

} // namespace intrinsics

} // namespace rt

} // namespace syslab
#endif