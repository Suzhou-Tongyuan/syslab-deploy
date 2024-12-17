#ifndef SYSLAB_CC_RUNTIME_UTILS_MISC
#define SYSLAB_CC_RUNTIME_UTILS_MISC

#include <cstdint>
#include <cstring>

/**
    此函数用于拷贝子字符串或者截断字符串。

    当buf_size < len时，优先拷贝靠后的字符。
 */
static inline void _sj_copy_substr_or_truncate(char *buf, std::int64_t buf_size, const char *str, std::int64_t start,
                                               std::int64_t len)
{
    if (str == nullptr || start < 0 || len < 0)
    {
        return;
    }
    // 拷贝子字符串

    if (buf_size < len)
    {
        // buf size不够时 优先拷贝靠后的字符
        std::memcpy(buf, str + start + len - buf_size + 1, buf_size - 1);
        buf[buf_size - 1] = '\0';
    }
    else
    {
        std::memcpy(buf, str + start, len);
        buf[len] = '\0';
    }
}

#endif