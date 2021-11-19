#pragma once

#include <filesystem>

#ifdef __linux__
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace ex {
namespace os {
inline std::filesystem::path homedir() {
#ifdef __APPLE__
  const char *homeEnv = std::getenv("HOME");
  return homeEnv;
#endif
#ifdef _WIN32
  const char *homeEnv = std::getenv("USERPROFILE");
  return homeEnv;
#endif
#ifdef __linux__
  auto uid = getuid();
  auto pw = getpwuid(uid);

  if (pw)
    return pw->pw_dir;
  return "";
#endif
}
} // namespace os
} // namespace ex