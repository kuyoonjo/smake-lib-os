#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
namespace ex {
namespace os {
inline auto exepath() {
  char path[MAX_PATH];
  GetModuleFileName(NULL, path, MAX_PATH);
  return std::filesystem::path(path);
}
} // namespace os
} // namespace ex
#endif
#ifdef __linux__
#include <unistd.h>
#include <filesystem>
namespace ex {
namespace os {
inline auto exepath() {
  char buf[4096] = {0};
  int r = (int)readlink("/proc/self/exe", buf, 4096);
  return r > 0 ? std::filesystem::path(buf) : std::filesystem::path();
}
} // namespace os
} // namespace ex

#endif
#ifdef __APPLE__
// #include <unistd.h>
#include <filesystem>
#include <mach-o/dyld.h>
namespace ex {
namespace os {
inline auto exepath() {
  char buf[4096] = {0};
  uint32_t size = sizeof(buf);
  int r = _NSGetExecutablePath(buf, &size);
  return r == 0 ? std::filesystem::path(buf) : std::filesystem::path();
}
} // namespace os
} // namespace ex
#endif