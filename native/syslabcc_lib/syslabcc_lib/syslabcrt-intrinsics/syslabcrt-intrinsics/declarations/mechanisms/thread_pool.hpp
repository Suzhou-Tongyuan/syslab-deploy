#ifndef SYSLABCRT_DECL_MECHAN_THREAD_POOL
#define SYSLABCRT_DECL_MECHAN_THREAD_POOL
#include "syslabcrt-intrinsics/declarations/mechanisms/objects.hpp"
#include <cstddef>
#include <cstdint>
#include <deque>

#if defined(SYSLABCRT_WIN_THREAD_POOL)
#include "syslabcrt-intrinsics/declarations/compat.hpp"
#include <windows.h>
#else
#include <atomic>
#include <mutex>

#endif

// todo(-): 把所有的std::atomic<bool>换成平台无关的SJ_ATOMIC_BOOL
#if defined(SYSLABCRT_WIN_THREAD_POOL)
#define _SJ_ATOMIC_BOOL volatile LONG
#else
#define _SJ_ATOMIC_BOOL std::atomic<bool>
#endif

namespace syslab
{
namespace rt
{

#if defined (SYSLABCRT_WIN_THREAD_POOL)
inline bool _sj_swap_atomic_bool_by_true(SJ_ATOMICSJ_ATOMIC_BOOL* val)
{
    return InterlockedExchange(val, 1) == 1;
}
#else
inline bool _sj_swap_atomic_bool_by_true(_SJ_ATOMIC_BOOL* val)
{
    return val->exchange(true);
}
#endif

struct _sj_threadfn_params_t
{
    ptr_t<sj_array_t> ptr_fun;
    ptr_t<sj_array_t> ptr_closure;
};

struct _sj_tls_data_t
{
    _sj_threadfn_params_t *gc_data;
    uint8_t (*execute)(_sj_tls_data_t *);
    std::size_t task_id;
    std::int32_t thread_id;

    _sj_tls_data_t();

    void set_success();
    void set_failure();

    bool is_success();
    bool is_failure();
    bool is_finished();

  private:
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    LONG finished;
    LONG success;
#else
    std::atomic<bool> finished;
    std::atomic<bool> success;
#endif
};

struct _sj_tls_t
{
    // 此指针被 new/delete 分配/释放，但是注册为 GC root
    struct _sj_tls_data_t *data;
};

class _sj_mutex_t
{
  public:
    _sj_mutex_t(const _sj_mutex_t &) = delete;
    _sj_mutex_t(_sj_mutex_t &&) = delete;

    ~_sj_mutex_t();

#if defined(SYSLABCRT_WIN_THREAD_POOL)
    CRITICAL_SECTION *_assure_init_win32();
    volatile LONG is_initialized;

    _sj_mutex_t()
    {
        critical_section = nullptr;
        is_initialized = 0;
    }

  private:
    CRITICAL_SECTION *critical_section;
#else
    _sj_mutex_t()
    {
        mutex = new std::mutex();
    }

    std::mutex *mutex;
#endif
};

class _sj_mutex_guard_t
{
  public:
    _sj_mutex_guard_t(const _sj_mutex_guard_t &) = delete;
    _sj_mutex_guard_t(_sj_mutex_guard_t &&) = delete;
    _sj_mutex_guard_t(_sj_mutex_t &mutex);

    ~_sj_mutex_guard_t();

  private:
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    CRITICAL_SECTION *critical_section;
#else
    std::mutex *mutex_ptr;
#endif
};

/// @brief 运行多个任务，直到指定任务全部完成
static inline void sj_thread_pool_run(std::deque<_sj_tls_t> &task_queue, std::int32_t nthreads);

// 平台无关的tls操作函数
// alloc free set_index 为win32线程池独有 在其他情况下为空操作

static inline void _sj_tls_index_alloc();

static inline void _sj_tls_set_tls_index(std::int64_t);

static inline void _sj_tls_set_taskid(std::int64_t);

static inline void _sj_tls_set_threadid(std::int64_t);

inline int64_t _sj_tls_get_threadid();

inline int64_t _sj_tls_get_taskid();
} // namespace rt
} // namespace syslab
#endif