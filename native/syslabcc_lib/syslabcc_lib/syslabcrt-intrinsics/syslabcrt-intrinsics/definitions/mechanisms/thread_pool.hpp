#ifndef SYSLABCRT_DEF_MECHAN_THREAD_POOL
#define SYSLABCRT_DEF_MECHAN_THREAD_POOL

#include "syslabcrt-intrinsics/declarations/intrinsics/io.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/gc.hpp"
#include "syslabcrt-intrinsics/declarations/mechanisms/thread_pool.hpp"
#include "syslabcrt-intrinsics/declarations/mod.hpp"
#include <cstdio>
#include <deque>
#include <stdexcept>
#include <vector>

#if defined(SYSLABCRT_WIN_THREAD_POOL)
#include "syslabcrt-intrinsics/declarations/compat.hpp"
#include <synchapi.h>
#include <windows.h>
#include <winnt.h>
#else
#include <atomic>
#include <chrono>
#include <thread>
#endif

namespace syslab
{
namespace rt
{

// TODO: 低版本 GCC 处理 thread local
#if defined(_MSC_VER)
static __declspec(thread) int _sj_tls_threadid = 0;
static __declspec(thread) int _sj_tls_taskid = 0;
#elif defined(SYSLABCRT_WIN_THREAD_POOL)
struct _sj_win_thread_pool_tls_pub_data
{
    std::int64_t thread_id;
    std::int64_t task_id;
};

static DWORD _sj_win_thread_pool_tls_Index;
// TLS INDEX对应的数据
static _sj_win_thread_pool_tls_pub_data sj_win_threadpool_tls_pub_data[SYSLABCC_THREADPOOLSIZE];
static LONG _sj_is_alloc_win_tls_index = 0;
#else
static thread_local int _sj_tls_threadid = 0;
static thread_local int _sj_tls_taskid = 0;
#endif

static inline void _sj_tls_index_alloc()
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    if (InterlockedCompareExchange(&_sj_is_alloc_win_tls_index, 0, 0) == 0)
    {
        _sj_win_thread_pool_tls_Index = TlsAlloc();
        if (_sj_win_thread_pool_tls_Index == TLS_OUT_OF_INDEXES)
        {
            fprintf(stderr, "Win32 alloc tls fail");
        }
        InterlockedExchange(&_sj_is_alloc_win_tls_index, 1);
    }
#else
    return;
#endif
}

static inline void _sj_tls_set_tls_index(std::int64_t i)
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    if (i >= 0 && i < SYSLABCC_THREADPOOLSIZE)
    {
        if (!TlsSetValue(_sj_win_thread_pool_tls_Index, reinterpret_cast<LPVOID>(sj_win_threadpool_tls_pub_data + i)))
        {
            fprintf(stderr, "Win32 Set tls fail");
        }
    }
#else
    return;
#endif
}
#if defined(SYSLABCRT_WIN_THREAD_POOL)
static DWORD WINAPI __stdcall _sj_thread_run(LPVOID arg);
#else
static void *_sj_thread_run(void *arg);
#endif

inline int64_t _sj_tls_get_threadid()
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    _sj_win_thread_pool_tls_pub_data *ptr_tls =
        reinterpret_cast<_sj_win_thread_pool_tls_pub_data *>(TlsGetValue(_sj_win_thread_pool_tls_Index));
    if (ptr_tls == NULL)
    {
        fprintf(stderr, "Win32 ptr_tls is null in get thread_id");
        return -1;
    }
    return ptr_tls->thread_id;
#else
    return _sj_tls_threadid;
#endif
}

inline int64_t _sj_tls_get_taskid()
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    _sj_win_thread_pool_tls_pub_data *ptr_tls =
        reinterpret_cast<_sj_win_thread_pool_tls_pub_data *>(TlsGetValue(_sj_win_thread_pool_tls_Index));
    if (ptr_tls == NULL)
    {
        fprintf(stderr, "Win32 ptr_tls is null in get taskid");
        return -1;
    }
    return ptr_tls->task_id;
#else
    return _sj_tls_taskid;
#endif
}

static inline void _sj_tls_set_threadid(std::int64_t thread_id)
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    _sj_win_thread_pool_tls_pub_data *ptr_tls =
        reinterpret_cast<_sj_win_thread_pool_tls_pub_data *>(TlsGetValue(_sj_win_thread_pool_tls_Index));
    if (ptr_tls == NULL)
    {
        fprintf(stderr, "Win32 ptr_tls is null in set threadid");
        return;
    }
    ptr_tls->thread_id = thread_id;
#else
    _sj_tls_threadid = thread_id;
#endif
}

static inline void _sj_tls_set_taskid(std::int64_t task_id)
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    _sj_win_thread_pool_tls_pub_data *ptr_tls =
        reinterpret_cast<_sj_win_thread_pool_tls_pub_data *>(TlsGetValue(_sj_win_thread_pool_tls_Index));
    if (ptr_tls == NULL)
    {
        fprintf(stderr, "Win32 ptr_tls is null in set taskid");
        return;
    }
    ptr_tls->task_id = task_id;
#else
    _sj_tls_taskid = task_id;
#endif
}

struct _sj_channel_t
{
    _sj_mutex_t mutex;
    std::deque<_sj_tls_t> tasks;

    std::size_t channel_id;
};

struct _sj_thread_t
{
    std::size_t idx;

    void join()
    {
#if defined(SYSLABCRT_WIN_THREAD_POOL)
        WaitForSingleObject(thread_handle, INFINITE);
#else
        if (thread.joinable())
        {
            thread.join();
        }
#endif
    }
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    bool done()
    {
        return InterlockedCompareExchange(&this->_done, 0, 0) != 0;
    }

    void set_done()
    {
        InterlockedExchange(&this->_done, 1);
    }

    _sj_thread_t(std::size_t idx)
    {
        this->idx = idx;
        this->_done = 0;
        thread_handle = CreateThread(NULL, 0, _sj_thread_run, this, 0, NULL);
    }
    ~_sj_thread_t()
    {
        join();
        if (thread_handle != NULL)
            CloseHandle(thread_handle);
    }
    HANDLE thread_handle;

  private:
    LONG _done;
#else

    _sj_thread_t(std::size_t idx) : idx(idx), _done(false)
    {
        thread = std::thread(_sj_thread_run, reinterpret_cast<void *>(this));
    }
    std::thread thread;

    bool done()
    {
        return _done;
    }

    void set_done()
    {
        _done = true;
    }

    ~_sj_thread_t()
    {
        join();
    }

  private:
    std::atomic<bool> _done;
#endif
};

static _sj_mutex_t _sj_thread_global_lock;

static std::vector<_sj_channel_t *> _sj_channels;
static std::vector<_sj_thread_t *> _sj_threads;

inline _sj_mutex_t::~_sj_mutex_t()
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    if (critical_section != nullptr)
    {
        DeleteCriticalSection(critical_section);
        delete critical_section;
    }
#else
    if (mutex != nullptr)
    {
        delete mutex;
    }
#endif
}

#if defined(SYSLABCRT_WIN_THREAD_POOL)
inline _sj_tls_data_t::_sj_tls_data_t()
{
    finished = 0;
    success = 0;
}

inline bool _sj_tls_data_t::is_finished()
{
    // use interlock
    return InterlockedCompareExchange(&finished, 0, 0) != 0;
}

inline bool _sj_tls_data_t::is_success()
{
    return InterlockedCompareExchange(&success, 0, 0) != 0;
}

inline bool _sj_tls_data_t::is_failure()
{
    return InterlockedCompareExchange(&success, 0, 0) == 0;
}

inline void _sj_tls_data_t::set_failure()
{
    InterlockedExchange(&finished, 1);
    InterlockedExchange(&success, 0);
}

inline void _sj_tls_data_t::set_success()
{
    InterlockedExchange(&finished, 1);
    InterlockedExchange(&success, 1);
}
#else
inline _sj_tls_data_t::_sj_tls_data_t()
{
    finished = false;
    success = false;
}

inline bool _sj_tls_data_t::is_finished()
{
    return finished;
}

inline bool _sj_tls_data_t::is_success()
{
    return success;
}

inline bool _sj_tls_data_t::is_failure()
{
    return !success;
}

inline void _sj_tls_data_t::set_failure()
{
    finished = true;
    success = false;
}

inline void _sj_tls_data_t::set_success()
{
    finished = true;
    success = true;
}
#endif

static void _sj_thread_assure_setup(std::int32_t nthreads)
{
    _sj_mutex_guard_t guard(_sj_thread_global_lock);
    if (_sj_threads.size() < nthreads)
    {
        for (std::size_t i = 0; i < nthreads; i++)
        {
            _sj_threads.push_back(nullptr);
        }
    }
    if (_sj_channels.size() < nthreads)
    {
        for (std::size_t i = 0; i < nthreads; i++)
        {
            _sj_channel_t *new_channel = new _sj_channel_t;
            new_channel->channel_id = i;
            _sj_channels.push_back(new_channel);
        }
    }
}

#if defined(SYSLABCRT_WIN_THREAD_POOL)
inline CRITICAL_SECTION *_sj_mutex_t::_assure_init_win32()
{
    if (critical_section == nullptr)
    {

        // a simple spin lock to guard the initialization
        int micro_count = 0;
        int macro_count = 0;
        // is_initialized is 0 when not initialized
        while (InterlockedCompareExchange(&is_initialized, 1, 0) != 0)
        {
            // spin
            if (micro_count++ > 1024)
            {
                Sleep(1);
                micro_count = 0;
                if (macro_count++ > 1024)
                {
                    // if the initialization is too slow, we should give up
                    fprintf(stderr, "Failed to initialize the critical section\n");
                    exit(1);
                    return nullptr;
                }
            }
        }

        // now we gain the lock
        if (critical_section == nullptr)
        {
            critical_section = new CRITICAL_SECTION();
            InitializeCriticalSection(critical_section);
        }
        // release the lock
        InterlockedExchange(&is_initialized, 0);

        // return the critical section
    }
    return critical_section;
}
#endif

inline _sj_mutex_guard_t::_sj_mutex_guard_t(_sj_mutex_t &mutex)
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    critical_section = mutex._assure_init_win32();
    if (critical_section)
        EnterCriticalSection(critical_section);
#else
    mutex_ptr = mutex.mutex;
    mutex_ptr->lock();
#endif
}

inline _sj_mutex_guard_t::~_sj_mutex_guard_t()
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    if (critical_section)
        LeaveCriticalSection(critical_section);
#else
    if (mutex_ptr)
        mutex_ptr->unlock();
#endif
}

static void _sj_thread_pool_add_tasks(std::deque<_sj_tls_t> &tasks, std::int32_t nthreads)
{
    _sj_thread_assure_setup(nthreads);
    std::int32_t idx = 0;
    for (auto &task : tasks)
    {
        auto thread_id = idx++ % nthreads;
        auto &channel = _sj_channels[thread_id];

        task.data->thread_id = thread_id + 1;
        {
            _sj_mutex_guard_t guard(channel->mutex);
            channel->tasks.push_back(task);
        }
    }
}

static void _sj_sleep_micro()
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    Sleep(0);
#else
    std::this_thread::sleep_for(std::chrono::microseconds(1));
#endif
}

static void _sj_sleep_macro()
{
#if defined(SYSLABCRT_WIN_THREAD_POOL)
    Sleep(1);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif
}

/// 已经初始化
static void _sj_thread_assure_run_once(std::int32_t nthreads)
{
    _sj_mutex_guard_t guard(_sj_thread_global_lock);
    for (std::size_t i = 0; i < nthreads; i++)
    {

        if (_sj_threads[i] != nullptr && _sj_threads[i]->done())
        {
            auto ptr = _sj_threads[i];
            delete ptr;
            _sj_threads[i] = nullptr;
        }
        if (_sj_threads[i] == nullptr)
        {
            _sj_threads[i] = new _sj_thread_t(i);
        }
    }
}

static inline void sj_thread_pool_run(std::deque<_sj_tls_t> &left_tasks, std::int32_t nthreads)
{
    _sj_tls_index_alloc();
    _sj_thread_pool_add_tasks(left_tasks, nthreads);
    _sj_thread_assure_run_once(nthreads);

    std::size_t micro_count = 0;
    std::size_t macro_count = 0;
    std::vector<_sj_tls_t> result;
    while (!left_tasks.empty())
    {
        if (micro_count++ > 1024)
        {
            _sj_sleep_micro();
            micro_count = 0;
            if (macro_count++ > 1024)
            {
                macro_count = 0;
                _sj_sleep_macro();
            }
        }
        auto n_tasks = left_tasks.size();

        // TODO: lock per task?
        for (std::size_t i = 0; i < n_tasks; i++)
        {
            _sj_tls_t task = left_tasks.front();
            left_tasks.pop_front();
            if (task.data->is_finished())
            {
                result.push_back(task);
            }
            else
            {
                left_tasks.push_back(task);
            }
        }
    }

    // todo: gather exception
    bool has_failure = false;
    for (auto &task : result)
    {
        has_failure |= task.data->is_failure();
    }
    if (has_failure)
    {
        throw std::runtime_error("Task failed");
    }
}

#if defined(SYSLABCRT_WIN_THREAD_POOL)
static DWORD WINAPI __stdcall _sj_thread_run(LPVOID arg)
#else
static void *_sj_thread_run(void *arg)
#endif
{
    auto thread = reinterpret_cast<_sj_thread_t *>(arg);
    _sj_tls_set_tls_index(thread->idx);
    _sj_tls_set_threadid(thread->idx + 1);
    auto channel = thread->idx;
    GC::ManagedThreadRegistered _managered_thread;
    // lock channels
    if (_sj_channels.size() <= channel)
    {
        thread->set_done();
        return 0;
    }

    auto ch = _sj_channels[channel];
    std::size_t micro_count = 0;
    std::size_t macro_count = 0;

    while (true)
    {
        _sj_mutex_guard_t guard(ch->mutex);
        if (ch->tasks.empty())
        {
            if (micro_count++ > 1024)
            {
                _sj_sleep_micro();
                micro_count = 0;
                if (macro_count++ > 1024)
                {
                    break;
                }
            }
            continue;
        }

        auto task = ch->tasks.front();
        try
        {
            auto success = task.data->execute(task.data);
            if (success)
            {
                task.data->set_success();
            }
            else
            {
                task.data->set_failure();
            }
        }
        catch (...)
        {
            task.data->set_failure();
        }
        ch->tasks.pop_front();
    }
    thread->set_done();
    return 0;
}

} // namespace rt
} // namespace syslab

#endif