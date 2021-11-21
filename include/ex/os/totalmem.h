#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
namespace ex {
namespace os {
inline unsigned long long totalmem() {
  MEMORYSTATUSEX status;
  status.dwLength = sizeof(status);
  GlobalMemoryStatusEx(&status);
  return status.ullTotalPhys;
}
} // namespace os
} // namespace ex
#else
#include <unistd.h>

namespace ex {
namespace os {
inline unsigned long long totalmem() {
  long pages = sysconf(_SC_PHYS_PAGES);
  long page_size = sysconf(_SC_PAGE_SIZE);
  return pages * page_size;
}
} // namespace os
} // namespace ex
#endif