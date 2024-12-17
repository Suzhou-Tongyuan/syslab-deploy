#include "utils.h"
#include "platform.h"

#ifdef _OS_WINDOWS_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
void sleep_ms(int ms)
{
    Sleep(ms);
}
#else
#include <unistd.h>
void sleep_ms(int ms)
{
    usleep(1000 * ms);
}
#endif
