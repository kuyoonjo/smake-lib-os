#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
namespace ex {
namespace os {
inline int pid() { return (int)GetCurrentProcessId(); }
} // namespace os
} // namespace ex
#else
#include <unistd.h>

namespace ex {
namespace os {
inline int pid() { return (int)getpid(); }
} // namespace os
} // namespace ex
#endif