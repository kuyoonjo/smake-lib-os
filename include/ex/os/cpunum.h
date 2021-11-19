#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
namespace ex {
namespace os {
inline int cpunum() {
    static int ncpu = []() {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        return (int) info.dwNumberOfProcessors;
    }();
    return ncpu;
}
} // namespace os
} // namespace ex
#else
#include <unistd.h>

namespace ex {
namespace os {
inline int cpunum() {
    static int ncpu = (int) sysconf(_SC_NPROCESSORS_ONLN);
    return ncpu;
}
} // namespace os
} // namespace ex
#endif