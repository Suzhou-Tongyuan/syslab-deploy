#ifndef SYSLABCRT_DEF_INTRIN_THREAD_CONTEXT_HPP
#define SYSLABCRT_DEF_INTRIN_THREAD_CONTEXT_HPP

#include "syslabcrt-intrinsics/declarations/compat.hpp"
#include "syslabcrt-intrinsics/declarations/intrinsics/sjthread.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/gc.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/objects.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/stableptr.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/thread_pool.hpp"
#include "syslabcrt-intrinsics/declarations/syscompat.hpp"
#include "syslabcrt-intrinsics/definitions/intrinsics/errors.hpp"
#include "syslabcrt-intrinsics/definitions/intrinsics/io.hpp"
#include "syslabcrt-intrinsics/definitions/mechanisms/sjstring.hpp"
#include "syslabcrt-intrinsics/definitions/mechanisms/thread_pool.hpp"
#include "syslabcrt-intrinsics/definitions/mod-pre.hpp"
#include "syslabcrt-intrinsics/utils/misc.hpp"
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <deque>
#include <exception>
#include <vector>

#if defined(SYSLABCRT_WIN_THREAD_POOL)
#include <windows.h>
#endif

namespace syslab
{
namespace rt
{

namespace intrinsics
{

static std::atomic<std::int64_t> sj_context_count(0);

static const char *_sj_debug_info_str;
static const uint8_t *_sj_debug_info_frame;

inline std::size_t _sj_get_current_threadid(void)
{
    return _sj_tls_get_threadid();
}

inline std::size_t _sj_get_current_taskid(void)
{
    return _sj_tls_get_threadid();
}

inline std::int64_t CXX11_CONSTEXPR _sj_thread_pool_size()
{
    return SYSLABCC_THREADPOOLSIZE;
}

inline void _sj_debugtrace_init(const char *src_str, const uint8_t *src_frame)
{
    if (src_str == nullptr || src_frame == nullptr)
    {
        return;
    }
    else
    {
        _sj_debug_info_str = src_str;
        _sj_debug_info_frame = src_frame;
    }
}

inline _sj_managed_thread_context::_sj_managed_thread_context()
{
    sj_context_count += 1;
    context.Raw = new _sj_thread_context(sj_context_count);
}

inline _sj_managed_thread_context::~_sj_managed_thread_context()
{
    delete context.Raw;
    context.Raw = nullptr;
}

inline _sj_managed_thread_context::_sj_managed_thread_context(_sj_managed_thread_context &&managed)
{
    context = managed.context;
    managed.context = nullptr;
}

inline void _sj_thread_context::_sj_exception_pop()

{
    auto root = this->_sj_get_exception_root();
    if (root->binding == nullptr)
    {
        throw std::runtime_error("Exception stack is not initialized");
    }

    root->binding = root->binding->next;
    // 1. the exception item is allocated by GC, so no need to free it
    // 2. we keep the previous 'binding->item' and do not set it to nullptr, because we allow users to keep the
    // exception stack
}

inline sj_any_t _sj_thread_context::_sj_exception_get()
{
    auto item = this->_sj_exception_current_item();
    return item->exception;
}

inline _sj_stack_frame_t _sj_thread_context::sj_get_frame_index()
{
    return _sj_stack_frame_t(this->_sj_callstack.size());
}

inline _sj_exception_stackroot *_sj_thread_context::_sj_get_exception_root()
{
    if (this->_exception_root == nullptr)
    {
        this->_exception_root = (_sj_exception_stackroot *)GC::MallocUncollectable(sizeof(_sj_exception_stackroot));
        _exception_root->binding = nullptr;
    }
    return this->_exception_root;
}

inline void _sj_thread_context::_sj_exception_push_jlval(sj_any_t exc)
{
    auto root = this->_sj_get_exception_root();

    auto stack = (_sj_exception_stack *)GC::MallocUncollectable(sizeof(_sj_exception_stack));
    stack->exception = exc;
    stack->backtrace = Trace::TraceOn();
    stack->next = root->binding;
    root->binding = stack;
}

inline void _sj_thread_context::_sj_exception_push(const std::exception &e)
{
    sj_any_t exc;
    if (SJException *sje = dynamic_cast<SJException *>(const_cast<std::exception *>(&e)))
    {
        exc = sje->julia_exception_value;
    }
    else
    {
        exc = _sj_c_exception_to_jlval(e);
    }
    this->_sj_exception_push_jlval(exc);
}

inline _sj_builtintype_bot _sj_thread_context::sj_rethrow()
{
    auto item = this->_sj_exception_current_item();
    throw SJException(item->exception);
}

inline _sj_builtintype_bot _sj_thread_context::sj_rethrow_other(sj_any_t exc)
{
    auto item = this->_sj_exception_current_item();
    item->exception = exc;
    throw SJException(item->exception);
}

inline _sj_exception_stack *_sj_thread_context::_sj_exception_current_item()
{
    auto root = this->_sj_get_exception_root();
    if (root->binding == nullptr)
    {
        throw std::runtime_error("Exception stack is not initialized");
    }
    return root->binding;
}

inline std::int32_t _sj_thread_context::_sj_errno()
{
    return _sj_errno_value;
}

inline void _sj_thread_context::_sj_set_errno(std::int32_t value)
{
    _sj_errno_value = value;
}

// TODO(sjh): better way to maintain stack trace
inline void _sj_thread_context::_sj_callstack_push()
{
    _sj_frame_t frame;
    this->_sj_callstack.push_back(frame);
}

inline void _sj_thread_context::_sj_callstack_pop()
{
    if (this->_sj_callstack.size() > 0)
    {
        this->_sj_callstack.pop_back();
    }
}

inline unused_t _sj_thread_context::_sj_callstack_store(_sj_stack_frame_t &st)
{
    // 保存错误堆栈
    // TODO(-)：由于只有一个全局的 _sj_error_callstack
    //          那么要求打印错误堆栈的操作只能在刚进入 Julia catch 块时调用
    //          或者将相关的错误堆栈保存在局部变量 st 内
    //          但是这样就要求生成 _sj_print_debugtrace 时同时传入一个 st
    this->_sj_error_callstack.clear();
    for (_sj_frame_t &frame : this->_sj_callstack)
    {
        this->_sj_error_callstack.push_back(frame);
    }

    // unwind，清理堆栈 (zdh)
    if (st.index <= this->_sj_callstack.size())
    {
        this->_sj_callstack.erase(_sj_callstack.begin() + st.index, this->_sj_callstack.end());
    }
    return unused_t();
}

inline unused_t _sj_thread_context::_sj_callstack_clear()
{
    this->_sj_error_callstack.clear();
    return unused_t();
}

inline unused_t _sj_thread_context::_sj_callstack_set_item(std::int64_t index, std::int64_t len)
{
    this->_sj_callstack.back().index = index;
    this->_sj_callstack.back().len = len;
    return unused_t();
}

// 算法链接:https://en.wikipedia.org/wiki/LEB128
inline void _sj_thread_context::_sj_decode_one_item(_sj_debuginfo_decode_state &state)
{

    std::uint32_t currentNumber = 0;
    std::uint32_t shift = 0;
    std::int64_t cursor = state.cursor;
    _sj_debuginfo_msg_index msg_index = FileNameStart;
    if (_sj_debug_info_frame != nullptr)
    {
        for (; cursor < state.end; cursor++)
        {
            uint8_t code = _sj_debug_info_frame[cursor];
            if (code & (uint8_t)0x80)
            {
                currentNumber |= (uint32_t)((code & (uint8_t)0x7f) << shift);
                shift += 7;
            }
            else // 单字节最高位为0 解码一个整数结束
            {
                currentNumber |= (uint32_t)((code & (uint8_t)0x7f) << shift);
                state.msg[msg_index] = currentNumber;
                // 一个数字解码完成 处理状态转移
                switch (msg_index)
                {
                case FileNameStart:
                    msg_index = FileNameLen;
                    break;
                case FileNameLen:
                    msg_index = FuncNameStart;
                    break;
                case FuncNameStart:
                    msg_index = FuncNameLen;
                    break;
                case FuncNameLen:
                    msg_index = LineNumber;
                    break;
                case LineNumber:
                    state.cursor = cursor + 1;
                    state.is_decode_success = Success;
                    return;
                }
                shift = 0;
                currentNumber = 0;
            }
        }
        rt::_sj_printf("Warning debug_info_str parser not match cursor:%d \n", state.cursor);
        state.is_decode_success = Fail; // decode fail
        return;
    }
    {
        rt::_sj_printf("Warning _sj_debug_info_frame is null\n");
        state.is_decode_success = Fail; // decode fail
        return;
    }
}

inline unused_t _sj_thread_context::_sj_print_decode_frame(std::int64_t start, std::int64_t len,
                                                           std::ostringstream *ptr_oss = nullptr)
{
    if (_sj_debug_info_str != nullptr)
    {
        char path_str[SYSLABCC_PRINTBUF_PATH_SIZE];
        char func_str[SYSLABCC_PRINTBUF_FUNC_SIZE];
        _sj_debuginfo_decode_state state(start, start + len);
        while (state.cursor < state.end)
        {
            _sj_decode_one_item(state);

            if (state.is_decode_success == Fail)
            {
                break;
            }
            _sj_copy_substr_or_truncate(path_str, SYSLABCC_PRINTBUF_PATH_SIZE, _sj_debug_info_str, state.msg[0],
                                        state.msg[1]);
            _sj_copy_substr_or_truncate(func_str, SYSLABCC_PRINTBUF_FUNC_SIZE, _sj_debug_info_str, state.msg[2],
                                        state.msg[3]);
            if (ptr_oss != nullptr)
            {
                (*ptr_oss) << " " << path_str << ":" << state.msg[4] << " at " << func_str << "()\n";
            }
            else
            {
                rt::_sj_printf("  %s:%d at %s()\n", path_str, state.msg[4], func_str);
                fflush(stdout);
            }
        }
    }
    else
    {
        rt::_sj_printf("Warning sj_debug_info_str is null\n");
        fflush(stdout);
    }
    return unused_t();
}

inline unused_t _sj_thread_context::sj_print_debugtrace()
{
    // auto root = _sj_get_exception_root();
    // auto binding = root->binding;
    // if (binding != nullptr)
    // {
    //     auto errorException = _sj_generic_short_repr_as_string(binding->exception);
    //     rt::_sj_printf("Error: %s\n", (char *)errorException.c_str());
    // }

#ifdef SYSLABCC_DEBUG_TRACE
    if (_sj_callstack.size() > 0)
    {
        rt::_sj_printf("stacktrace(most recent exception):\n");
        fflush(stdout);

        for (int i = this->_sj_error_callstack.size() - 1; i >= 0; i--)
        {
            _sj_print_decode_frame(this->_sj_error_callstack[i].index, this->_sj_error_callstack[i].len, nullptr);
        }
    }
#endif
    return unused_t();
}

// 本方法仅用于debug
inline ptr_t<uint8_t> _sj_thread_context::sj_string_of_debugtrace()
{
    std::ostringstream buffer;

    if (this->_sj_callstack.size() > 0)
    {
        buffer << "stacktrace(most recent exception):\n";
        for (int i = this->_sj_error_callstack.size() - 1; i >= 0; i--)
        {
            //(lhy)todo string of unzip
            _sj_print_decode_frame(this->_sj_error_callstack[i].index, this->_sj_error_callstack[i].len, &buffer);
        }
    }

    std::string content = buffer.str();
    ptr_t<uint8_t> c_str_ptr = ptr_t<uint8_t>((void *)content.c_str());
    return sj_cstr_to_string(c_str_ptr);
}

inline unused_t _sj_thread_context::sj_print_trace()
{
    auto root = _sj_get_exception_root();
    auto binding = root->binding;
    if (binding != nullptr)
    {
        auto errorException = _sj_generic_short_repr_as_string(binding->exception);
        rt::_sj_printf("Error: %s\n", (char *)errorException.c_str());
    }

#ifdef SYSLABCC_DEBUG_TRACE
    if (_sj_callstack.size() > 0)
    {
        rt::_sj_printf("stacktrace:\n");
        fflush(stdout);

        for (int i = this->_sj_callstack.size() - 1; i >= 0; i--)
        {
            _sj_print_decode_frame(_sj_callstack[i].index, this->_sj_callstack[i].len, nullptr);
        }
    }
#endif
    return unused_t();
}
typedef uint8_t (*_sj_threadforfun_fptr_t)(ptr_t<sj_array_t> ptr_closure, std::int64_t tid);

static uint8_t _sj_threadfor_exec(_sj_tls_data_t *data)
{
    _sj_threadfn_params_t *params = (_sj_threadfn_params_t *)data->gc_data;
    int64_t tid = data->task_id;
    _sj_tls_set_taskid(data->task_id);

    _sj_threadforfun_fptr_t callback = ((_sj_threadforfun_fptr_t *)((params->ptr_fun.Raw)->data))[0];
    try
    {
        auto r = callback(params->ptr_closure, tid + 1);
        return r;
    }
    catch (std::exception &e)
    {
        // todo: 保存错误堆栈
        fprintf(stderr, "Error: unhandled exception in threadfor\n");
        return 0;
    }
}

struct _sj_temporary_task_queue
{
    std::deque<_sj_tls_t> queue;

    _sj_temporary_task_queue() : queue()
    {
    }

    _sj_tls_t new_task()
    {
        _sj_tls_t task;
        task.data = new _sj_tls_data_t;
        auto gc_data_ptr = (_sj_threadfn_params_t *)GC::Malloc(sizeof(_sj_threadfn_params_t));
        task.data->gc_data = gc_data_ptr;
        task.data->execute = _sj_threadfor_exec;
        GC::AddRoots(gc_data_ptr, gc_data_ptr + 1);
        this->queue.push_back(task);
        return task;
    }

    ~_sj_temporary_task_queue()
    {
        for (auto &task : queue)
        {
            GC::RemoveRoots(task.data->gc_data, task.data->gc_data + 1);
            delete task.data;
        }
    }
};

inline unused_t _sj_threadfor_run(ptr_t<sj_array_t> ptr_fun, ptr_t<sj_array_t> ptr_closure)
{
    _sj_temporary_task_queue task_queue;
    for (std::size_t i = 0; i < SYSLABCC_THREADPOOLSIZE; i++)
    {
        //  t = Task(() -> fun(i))
        //  schedule(t)

        _sj_tls_t task = task_queue.new_task();
        task.data->task_id = i;
        task.data->thread_id = 0;
        task.data->gc_data->ptr_fun = ptr_fun;
        task.data->gc_data->ptr_closure = ptr_closure;
    }

    sj_thread_pool_run(task_queue.queue, SYSLABCC_THREADPOOLSIZE);
    return unused_t();
}

inline ptr_t<std::uint8_t> _sj_thread_context::sj_sprint_trace()
{
    // TODO(+): stacktrace
    auto root = _sj_get_exception_root();
    auto binding = root->binding;
    if (binding != nullptr)
    {
        auto errorException = _sj_generic_short_repr_as_string(binding->exception);
        return sj_cstr_to_string(ptr_t<uint8_t>((uint8_t *)errorException.c_str()));
    }
    else
    {
        const char *str = "";
        return sj_cstr_to_string(ptr_t<uint8_t>((uint8_t *)str));
    }
}
} // namespace intrinsics

} // namespace rt
} // namespace syslab

#endif